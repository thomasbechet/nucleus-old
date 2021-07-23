#ifndef NU_IO_H
#define NU_IO_H

#include <nucleus/core/common/common.h>

NU_DECLARE_HANDLE(nu_file_t);

typedef enum {
    NU_IO_READ_ONLY
} nu_io_mode_t;

NU_API nu_result_t nu_file_open(const char *filename, nu_file_t *file, nu_io_mode_t mode);
NU_API nu_result_t nu_file_close(nu_file_t file);
NU_API bool nu_file_readline(nu_file_t file, uint32_t n, char *buf);

#endif