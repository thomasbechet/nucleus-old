#include <nucleus/core/utils/io.h>

#include <nucleus/core/common/platform.h>
#include <nucleus/core/coresystem/memory/memory.h>
#include <nucleus/core/utils/path.h>

#if defined(NU_PLATFORM_UNIX)
/* taken from https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950 */
static nu_result_t create_directory(const char *dir)
{
    const size_t len = strlen(dir);
    char _dir[PATH_MAX];
    char *p;

    /* copy string so its mutable */
    if (len > sizeof(_dir) - 1) {
        return NU_FAILURE;
    }   
    strcpy(_dir, dir);

    /* iterate the string */
    for (p = _dir + 1; *p; p++) {
        if (*p == '/') {
            /* temporarily truncate */
            *p = '\0';

            if (mkdir(_dir, S_IRWXU) != 0) {
                if (errno != EEXIST)
                    return NU_FAILURE; 
            }

            *p = '/';
        }
    }   

    if (mkdir(_dir, S_IRWXU) != 0) {
        if (errno != EEXIST) {
            return NU_FAILURE;
        }
    }

    return NU_SUCCESS;
}
#elif defined(NU_PLATFORM_WINDOWS)
#include <windows.h>
static nu_result_t create_directory(const char *dir)
{
    CreateDirectory(dir, NULL);
    return NU_SUCCESS;
}
#endif

nu_result_t nu_file_open(nu_path_t path, nu_io_mode_t mode, nu_file_t *file)
{
    if (!nu_path_is_filename(path)) return NU_FAILURE;

    FILE *fp;
    switch (mode) {
        case NU_IO_MODE_READ:
            fp = fopen(nu_path_get_cstr(path), "rb");
            break;
        case NU_IO_MODE_WRITE: {
            nu_path_t directory;
            nu_path_get_directory(path, &directory);
            nu_result_t result = create_directory(nu_path_get_cstr(directory));
            nu_path_free(directory);
            if (result != NU_SUCCESS) return NU_FAILURE;
            fp = fopen(nu_path_get_cstr(path), "wb");
            break;
        }
        case NU_IO_MODE_APPEND:
            fp = fopen(nu_path_get_cstr(path), "wb+");
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
nu_result_t nu_file_readall_bytes(nu_path_t path, uint32_t *nbytes, char **buf)
{
    if (!nu_path_is_filename(path)) return NU_FAILURE;

    FILE *fp = fopen(nu_path_get_cstr(path), "rb");
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
nu_result_t nu_file_readall_string(nu_path_t path, nu_string_t *str)
{
    FILE *fp = fopen(nu_path_get_cstr(path), "rb");
    if (!fp) return NU_FAILURE;

    fseek(fp, 0, SEEK_END);
    uint32_t nbytes = ftell(fp);
    rewind(fp);

    if (nbytes == 0) return NU_FAILURE;

    nu_string_allocate_length(str, nbytes);
    fread(nu_string_get_data(*str), nbytes, 1, fp);

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
nu_result_t nu_file_write_string(nu_file_t file, nu_string_t str)
{
    fputs(nu_string_get_cstr(str), (FILE*)file);
    
    return NU_SUCCESS;
}