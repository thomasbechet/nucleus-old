#include <nucleus/core/utils/io.h>

#include <nucleus/core/coresystem/memory/memory.h>

nu_result_t nu_file_open(const char *filename, nu_io_mode_t mode, nu_file_t *file)
{
    
    FILE *fp;
    switch (mode) {
        case NU_IO_MODE_READ:
            fp = fopen(filename, "rb");
            break;
        case NU_IO_MODE_WRITE:
            fp = fopen(filename, "wb");
            break;
        case NU_IO_MODE_APPEND:
            fp = fopen(filename, "wb+");
            break;
        default:
            break;
    }
    if (!fp) return NU_FAILURE;
    *((FILE**)file) = fp;
    return NU_SUCCESS;
}
nu_result_t nu_file_close(nu_file_t file)
{
    fclose((FILE*)file);
    return NU_SUCCESS;
}
bool nu_file_read_line(nu_file_t file, uint32_t n, char *buf)
{
    return fgets(buf, n, (FILE*)file) != NULL;
}
nu_result_t nu_file_read_allbytes(const char *filename, uint32_t *nbytes, char **buf)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NU_FAILURE;

    fseek(fp, 0, SEEK_END);
    *nbytes = ftell(fp);
    rewind(fp);

    if (*nbytes == 0) return NU_FAILURE;

    *buf = (char*)nu_malloc(*nbytes * sizeof(char));
    fread(*buf, *nbytes, 1, fp);

    fclose(fp);

    return NU_SUCCESS;
}
nu_result_t nu_file_write_vprintf(nu_file_t file, const char *format, va_list args)
{
    vfprintf((FILE*)file, format, args);
    return NU_SUCCESS;
}
nu_result_t nu_file_write_printf(nu_file_t file, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_file_write_vprintf(file, format, args);
    va_end(args);

    return NU_SUCCESS;
}