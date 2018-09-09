/*
 * The volume superblock functions
 *
 * Copyright (C) 2018, Joachim Metz <joachim.metz@gmail.com>
 *
 * Refer to AUTHORS for acknowledgements.
 *
 * This software is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this software.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <common.h>
#include <byte_stream.h>
#include <memory.h>
#include <types.h>

#include "libfsapfs_volume_superblock.h"
#include "libfsapfs_debug.h"
#include "libfsapfs_io_handle.h"
#include "libfsapfs_libbfio.h"
#include "libfsapfs_libcerror.h"
#include "libfsapfs_libcnotify.h"
#include "libfsapfs_libfguid.h"

#include "fsapfs_volume_superblock.h"

/* Creates a volume superblock
 * Make sure the value volume_superblock is referencing, is set to NULL
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_superblock_initialize(
     libfsapfs_volume_superblock_t **volume_superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_volume_superblock_initialize";

	if( volume_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume superblock.",
		 function );

		return( -1 );
	}
	if( *volume_superblock != NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_ALREADY_SET,
		 "%s: invalid volume superblock value already set.",
		 function );

		return( -1 );
	}
	*volume_superblock = memory_allocate_structure(
	                      libfsapfs_volume_superblock_t );

	if( *volume_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_INSUFFICIENT,
		 "%s: unable to create volume superblock.",
		 function );

		goto on_error;
	}
	if( memory_set(
	     *volume_superblock,
	     0,
	     sizeof( libfsapfs_volume_superblock_t ) ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_SET_FAILED,
		 "%s: unable to clear volume superblock.",
		 function );

		goto on_error;
	}
	return( 1 );

on_error:
	if( *volume_superblock != NULL )
	{
		memory_free(
		 *volume_superblock );

		*volume_superblock = NULL;
	}
	return( -1 );
}

/* Frees a volume superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_superblock_free(
     libfsapfs_volume_superblock_t **volume_superblock,
     libcerror_error_t **error )
{
	static char *function = "libfsapfs_volume_superblock_free";

	if( volume_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume superblock.",
		 function );

		return( -1 );
	}
	if( *volume_superblock != NULL )
	{
		memory_free(
		 *volume_superblock );

		*volume_superblock = NULL;
	}
	return( 1 );
}

/* Reads the volume superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_superblock_read_file_io_handle(
     libfsapfs_volume_superblock_t *volume_superblock,
     libbfio_handle_t *file_io_handle,
     off64_t file_offset,
     libcerror_error_t **error )
{
	fsapfs_volume_superblock_t volume_superblock_data;

	static char *function = "libfsapfs_volume_superblock_read_file_io_handle";
	ssize_t read_count    = 0;

	if( volume_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume superblock.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: reading volume superblock at offset: %" PRIi64 " (0x%08" PRIx64 ")\n",
		 function,
		 file_offset,
		 file_offset );
	}
#endif
	if( libbfio_handle_seek_offset(
	     file_io_handle,
	     file_offset,
	     SEEK_SET,
	     error ) == -1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_SEEK_FAILED,
		 "%s: unable to seek volume superblock offset: %" PRIi64 " (0x%08" PRIx64 ").",
		 function,
		 file_offset,
		 file_offset );

		return( -1 );
	}
	read_count = libbfio_handle_read_buffer(
	              file_io_handle,
	              (uint8_t *) &volume_superblock_data,
	              sizeof( fsapfs_volume_superblock_t ),
	              error );

	if( read_count != (ssize_t) sizeof( fsapfs_volume_superblock_t ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume superblock data.",
		 function );

		return( -1 );
	}
	if( libfsapfs_volume_superblock_read_data(
	     volume_superblock,
	     (uint8_t *) &volume_superblock_data,
	     sizeof( fsapfs_volume_superblock_t ),
	     error ) != 1 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_IO,
		 LIBCERROR_IO_ERROR_READ_FAILED,
		 "%s: unable to read volume superblock data.",
		 function );

		return( -1 );
	}
	return( 1 );
}

/* Reads the volume superblock
 * Returns 1 if successful or -1 on error
 */
int libfsapfs_volume_superblock_read_data(
     libfsapfs_volume_superblock_t *volume_superblock,
     const uint8_t *data,
     size_t data_size,
     libcerror_error_t **error )
{
	static char *function   = "libfsapfs_volume_superblock_read_data";
	uint32_t object_subtype = 0;
	uint32_t object_type    = 0;

#if defined( HAVE_DEBUG_OUTPUT )
	uint64_t value_64bit    = 0;
	uint32_t value_32bit    = 0;
#endif

	if( volume_superblock == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_ARGUMENTS,
		 LIBCERROR_ARGUMENT_ERROR_INVALID_VALUE,
		 "%s: invalid volume superblock.",
		 function );

		return( -1 );
	}
	if( data == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_MISSING,
		 "%s: invalid data.",
		 function );

		return( -1 );
	}
	if( ( data_size < sizeof( fsapfs_volume_superblock_t ) )
	 || ( data_size > (size_t) SSIZE_MAX ) )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_VALUE_OUT_OF_BOUNDS,
		 "%s: invalid data size value out of bounds.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		libcnotify_printf(
		 "%s: volume superblock data:\n",
		 function );
		libcnotify_print_data(
		 data,
		 sizeof( fsapfs_volume_superblock_t ),
		 LIBCNOTIFY_PRINT_DATA_FLAG_GROUP_DATA );
	}
#endif
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_volume_superblock_t *) data )->object_type,
	 object_type );

	if( object_type != 0x0000000dUL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object type: 0x%08" PRIx32 ".",
		 function,
		 object_type );

		return( -1 );
	}
	byte_stream_copy_to_uint32_little_endian(
	 ( (fsapfs_volume_superblock_t *) data )->object_subtype,
	 object_subtype );

	if( object_subtype != 0x00000000UL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid object subtype: 0x%08" PRIx32 ".",
		 function,
		 object_subtype );

		return( -1 );
	}
	if( memory_compare(
	     ( (fsapfs_volume_superblock_t *) data )->signature,
	     fsapfs_volume_signature,
	     4 ) != 0 )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_RUNTIME,
		 LIBCERROR_RUNTIME_ERROR_UNSUPPORTED_VALUE,
		 "%s: invalid signature.",
		 function );

		return( -1 );
	}
	if( memory_copy(
	     volume_superblock->volume_identifier,
	     ( (fsapfs_volume_superblock_t *) data )->volume_identifier,
	     16 ) == NULL )
	{
		libcerror_error_set(
		 error,
		 LIBCERROR_ERROR_DOMAIN_MEMORY,
		 LIBCERROR_MEMORY_ERROR_COPY_FAILED,
		 "%s: unable to copy volume identifier.",
		 function );

		return( -1 );
	}
#if defined( HAVE_DEBUG_OUTPUT )
	if( libcnotify_verbose != 0 )
	{
		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->object_checksum,
		 value_64bit );
		libcnotify_printf(
		 "%s: object checksum\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: object identifier\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->object_version,
		 value_64bit );
		libcnotify_printf(
		 "%s: object version\t\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		libcnotify_printf(
		 "%s: object type\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_type );

		libcnotify_printf(
		 "%s: object subtype\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 object_subtype );

		libcnotify_printf(
		 "%s: signature\t\t\t\t: %c%c%c%c\n",
		 function,
		 ( (fsapfs_volume_superblock_t *) data )->signature[ 0 ],
		 ( (fsapfs_volume_superblock_t *) data )->signature[ 1 ],
		 ( (fsapfs_volume_superblock_t *) data )->signature[ 2 ],
		 ( (fsapfs_volume_superblock_t *) data )->signature[ 3 ] );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown1,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown1\t\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->compatible_features_flags,
		 value_64bit );
		libcnotify_printf(
		 "%s: compatible features flags\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->read_only_compatible_features_flags,
		 value_64bit );
		libcnotify_printf(
		 "%s: read-only compatible features flags\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->incompatible_features_flags,
		 value_64bit );
		libcnotify_printf(
		 "%s: incompatible features flags\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown5,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown5\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->number_of_reserved_blocks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of reserved blocks\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->number_of_quota_blocks,
		 value_64bit );
		libcnotify_printf(
		 "%s: number of quota blocks\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown8,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown8\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown9,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown9\t\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown10,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown10\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown11,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown11\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown12,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown12\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown13,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown13\t\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown14,
		 value_32bit );
		libcnotify_printf(
		 "%s: unknown14 object type\t\t\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint32_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->snapshot_metadata_tree_object_type,
		 value_32bit );
		libcnotify_printf(
		 "%s: snapshot metadata tree object type\t: 0x%08" PRIx32 "\n",
		 function,
		 value_32bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->object_map_block_number,
		 value_64bit );
		libcnotify_printf(
		 "%s: object map block number\t\t\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->file_system_root_object_identifier,
		 value_64bit );
		libcnotify_printf(
		 "%s: file system root object identifier\t: %" PRIu64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown18,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown18\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown19,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown19\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown20,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown20\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown21,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown21\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown22,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown22\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown23,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown23\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown24,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown24\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown25,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown25\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown26,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown26\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown27,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown27\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown28,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown28\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		byte_stream_copy_to_uint64_little_endian(
		 ( (fsapfs_volume_superblock_t *) data )->unknown29,
		 value_64bit );
		libcnotify_printf(
		 "%s: unknown29\t\t\t\t: 0x%08" PRIx64 "\n",
		 function,
		 value_64bit );

		if( libfsapfs_debug_print_guid_value(
		     function,
		     "volume identifier\t\t\t",
		     ( (fsapfs_volume_superblock_t *) data )->volume_identifier,
		     16,
		     LIBFGUID_ENDIAN_BIG,
		     LIBFGUID_STRING_FORMAT_FLAG_USE_LOWER_CASE,
		     error ) != 1 )
		{
			libcerror_error_set(
			 error,
			 LIBCERROR_ERROR_DOMAIN_RUNTIME,
			 LIBCERROR_RUNTIME_ERROR_PRINT_FAILED,
			 "%s: unable to print UUID value.",
			 function );

			return( -1 );
		}

		libcnotify_printf(
		 "\n" );
	}
#endif /* defined( HAVE_DEBUG_OUTPUT ) */

	return( 1 );
}

