#include <nucleus/core/utils/io.h>

#include <nucleus/core/coresystem/memory/memory.h>

nu_result_t nu_file_open(const char *filename, nu_file_t *file, nu_io_mode_t mode)
{
    (void)mode;
    FILE *fp = fopen(filename, "r");
    if (!fp) return NU_FAILURE;
    *((FILE**)file) = fp;
    return NU_SUCCESS;
}
nu_result_t nu_file_close(nu_file_t file)
{
    fclose((FILE*)file);
    return NU_SUCCESS;
}
bool nu_file_readline(nu_file_t file, uint32_t n, char *buf)
{
    return fgets(buf, n, (FILE*)file) != NULL;
}