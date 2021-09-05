#ifndef NU_IO_H
#define NU_IO_H

#include <nucleus/core/common/common.h>
#include <nucleus/core/utils/string.h>
#include <nucleus/core/utils/path.h>

NU_DECLARE_HANDLE(nu_file_t);

typedef enum {
    NU_IO_MODE_READ,
    NU_IO_MODE_WRITE,
    NU_IO_MODE_APPEND
} nu_io_mode_t;

NU_API nu_result_t nu_file_open(nu_path_t path, nu_io_mode_t mode, nu_file_t *file);
NU_API nu_result_t nu_file_close(nu_file_t file);
NU_API nu_result_t nu_file_readall_bytes(nu_path_t path, uint32_t *nbytes, char **buf);
NU_API nu_result_t nu_file_readall_string(nu_path_t path, nu_string_t *str);
NU_API nu_result_t nu_file_write_vprintf(nu_file_t file, const char *format, va_list args);
NU_API nu_result_t nu_file_write_printf(nu_file_t file, const char *format, ...);
NU_API nu_result_t nu_file_write_string(nu_file_t file, nu_string_t str);

#endif