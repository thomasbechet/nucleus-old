#ifndef NU_IO_H
#define NU_IO_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_file_t);

typedef enum {
    NU_IO_MODE_READ,
    NU_IO_MODE_WRITE,
    NU_IO_MODE_APPEND
} nu_io_mode_t;

NU_API nu_result_t nu_file_open(const char *filename, nu_io_mode_t mode, nu_file_t *file);
NU_API nu_result_t nu_file_close(nu_file_t file);
NU_API bool nu_file_read_line(nu_file_t file, uint32_t n, char *buf);
NU_API nu_result_t nu_file_read_allbytes(const char *filename, uint32_t *nbytes, char **buf);
NU_API nu_result_t nu_file_write_vprintf(nu_file_t file, const char *format, va_list args);
NU_API nu_result_t nu_file_write_printf(nu_file_t file, const char *format, ...);

#endif