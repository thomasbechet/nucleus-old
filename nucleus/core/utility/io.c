#include <nucleus/core/utility/io.h>

#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>

#if defined(NU_PLATFORM_UNIX)
// Taken from https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950
#include <limits.h>
#include <sys/stat.h>
#include <errno.h>
static nu_result_t create_directory(const char *dir)
{
    const size_t len = strlen(dir);
    char _dir[PATH_MAX];
    char *p;

    // Copy string so its mutable
    if (len > sizeof(_dir) - 1) {
        return NU_FAILURE;
    }   
    strcpy(_dir, dir);

    // Iterate the string
    for (p = _dir + 1; *p; p++) {
        if (*p == '/') {
            // Temporarily truncate
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

nu_file_t nu_file_open(const char *filename, nu_io_mode_t mode)
{
    FILE *fp;
    switch (mode) {
        case NU_IO_MODE_READ:
            fp = fopen(filename, "rb");
            break;
        case NU_IO_MODE_WRITE: {
            nu_string_t directory = nu_string_allocate_directory(nu_allocator_get_core(), filename);
            nu_result_t result    = create_directory(directory);
            nu_string_free(directory);
            if (result != NU_SUCCESS) return NU_NULL_HANDLE;
            fp = fopen(filename, "wb");
            break;
        }
        case NU_IO_MODE_APPEND:
            fp = fopen(filename, "wb+");
            break;
        default:
            fp = NULL;
            break;
    }
    if (!fp) return NU_NULL_HANDLE;
    return (nu_file_t)fp;
}
nu_result_t nu_file_close(nu_file_t file)
{
    fclose((FILE*)file);
    return NU_SUCCESS;
}
nu_result_t nu_file_copy(const char *src, const char *dst)
{
    
}
int8_t *nu_io_readall_bytes(nu_allocator_t allocator, const char *filename, uint32_t *nbytes)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;

    fseek(fp, 0, SEEK_END);
    *nbytes = ftell(fp);
    rewind(fp);

    if (*nbytes == 0) return NULL;

    int8_t *buf = nu_malloc(allocator, *nbytes * sizeof(*buf));
    fread(buf, *nbytes, 1, fp);

    fclose(fp);

    return buf;
}
nu_string_t nu_io_readall_string(nu_allocator_t allocator, const char* filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NU_NULL_HANDLE;

    fseek(fp, 0, SEEK_END);
    uint32_t nbytes = ftell(fp);
    rewind(fp);

    if (nbytes == 0) return NU_NULL_HANDLE;

    nu_string_t str = nu_string_allocate_capacity(allocator, nbytes);
    fread(str, nbytes, 1, fp);

    fclose(fp);

    return str;
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
    fputs(str, (FILE*)file);
    
    return NU_SUCCESS;
}
nu_result_t nu_file_write_cstr(nu_file_t file, const char *cstr)
{
    fputs(cstr, (FILE*)file);
    
    return NU_SUCCESS;
}