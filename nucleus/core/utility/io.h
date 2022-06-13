#ifndef NU_IO_H
#define NU_IO_H

#include <nucleus/core/utility/platform.h>
#include <nucleus/core/utility/macro.h>
#include <nucleus/core/utility/result.h>
#include <nucleus/core/utility/string.h>

NU_DECLARE_HANDLE(nu_file_t);

typedef enum {
    NU_IO_MODE_READ,
    NU_IO_MODE_WRITE,
    NU_IO_MODE_APPEND
} nu_io_mode_t;

NU_API nu_file_t nu_file_open(const char *filename, nu_io_mode_t mode);
NU_API nu_result_t nu_file_close(nu_file_t file);
NU_API nu_result_t nu_file_copy(const char *src, const char *dst);
NU_API nu_result_t nu_file_remove(const char *filename);
NU_API nu_result_t nu_file_last_write_time(const char *filename);
NU_API int8_t *nu_io_readall_bytes(nu_allocator_t allocator, const char *filename, uint32_t *nbytes);
NU_API nu_string_t nu_io_readall_string(nu_allocator_t allocator, const char *filename);
NU_API nu_result_t nu_file_write_vprintf(nu_file_t file, const char *format, va_list args);
NU_API nu_result_t nu_file_write_printf(nu_file_t file, const char *format, ...);
NU_API nu_result_t nu_file_write_string(nu_file_t file, nu_string_t str);
NU_API nu_result_t nu_file_write_cstr(nu_file_t file, const char *cstr);

#endif