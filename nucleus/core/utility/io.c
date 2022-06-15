#include <nucleus/core/utility/io.h>

#include <nucleus/core/system/allocator/allocator.h>
#include <nucleus/core/system/allocator/api.h>
#include <nucleus/core/system/logger/logger.h>
#include <nucleus/core/system/logger/api.h>

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
static nu_time_t last_write_time(const char *path)
{
    struct stat stats;
    if (stat(path, &stats) == -1) {
        return -1;
    }
    if (stats.st_size == 0) {
        return -1;
    }
    return stats.st_mtim.tv_sec;
}
#elif defined(NU_PLATFORM_WINDOWS)
#include <windows.h>
static nu_result_t create_directory(const char *dir)
{
    CreateDirectory(dir, NULL);
    return NU_SUCCESS;
}
static nu_time_t last_write_time(const char *path)
{
    WIN32_FILE_ATTRIBUTE_DATA attributes;
    if (!GetFileAttributesEx(path, GetFileExInfoStandard, &attributes)) {
        return 0;
    }
    if (attributes.nFileSizeHigh == 0 && attributes.nFileSizeLow == 0) {
        return 0;
    }
    LARGE_INTEGER time;
    time.HighPart = attributes.ftLastWriteTime.dwHighDateTime;
    time.LowPart  = attributes.ftLastWriteTime.dwLowDateTime;
    return (nu_time_t)time.QuadPart / 10000000 - 11644473600LL;
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
    nu_file_t f_src = nu_file_open(src, NU_IO_MODE_READ);
    NU_CHECK(f_src != NU_NULL_HANDLE, return NU_FAILURE, nu_logger_get_core(),
        "Failed to open source '%s'.", src);
    nu_file_t f_dst = nu_file_open(dst, NU_IO_MODE_WRITE);
    NU_CHECK(f_dst != NU_NULL_HANDLE, goto cleanup0, nu_logger_get_core(),
        "Failed to open destination '%s'.", dst);

    int a;
    while ((a = fgetc((FILE*)f_src)) != EOF) {
        fputc(a, (FILE*)f_dst);
    }

    nu_file_close(f_dst);
cleanup0:
    nu_file_close(f_src);

    return NU_SUCCESS;
}
nu_result_t nu_file_remove(const char *filename)
{
    return remove(filename) == 0 ? NU_SUCCESS : NU_FAILURE; 
}
nu_result_t nu_file_last_write_time(const char *filename)
{
    return last_write_time(filename);
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
    size_t total = fread(buf, *nbytes, 1, fp);
    fclose(fp);
    if (total != *nbytes) return NULL;
    return buf;
}
nu_string_t nu_io_readall_string(nu_allocator_t allocator, const char* filename)
{
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NU_NULL_HANDLE;

    fseek(fp, 0, SEEK_END);
    size_t nbytes = ftell(fp);
    rewind(fp);

    if (nbytes == 0) return NU_NULL_HANDLE;

    nu_string_t str = nu_string_allocate_capacity(allocator, (uint32_t)nbytes);
    size_t total = fread(str, nbytes, 1, fp);
    fclose(fp);
    if (total != nbytes) {
        nu_string_free(str);
        return NU_NULL_HANDLE;
    }
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