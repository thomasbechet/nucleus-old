#include <nucleus/core/utility/io/path.h>

static void nu_path_sanatize(nu_string_t *str)
{
    nu_string_replace_cstr(str, "$ENGINE_DIR", NU_PATH_ENGINE_DIRECTORY);
    nu_string_replace_cstr(str, "$MODULE_DIR", NU_PATH_MODULE_DIRECTORY);
    nu_string_replace_cstr(str, "$ROOT_DIR", NU_PATH_ROOT_DIRECTORY);

    nu_string_replace_cstr(str, "\\", NU_PATH_SEPARATOR);
    nu_string_replace_cstr(str, "//", NU_PATH_SEPARATOR);
    if (nu_string_get_cstr(*str)[0] == NU_PATH_SEPARATOR[0]) {
        nu_string_erase(str, 0, 1);
    }
}

void nu_path_allocate(nu_path_t *handle)
{
    nu_string_allocate((nu_string_t*)handle);
}
void nu_path_allocate_cstr(const char *path, nu_path_t *handle)
{
    nu_string_allocate_cstr(path, (nu_string_t*)handle);
    nu_path_sanatize((nu_string_t*)handle);
}
void nu_path_allocate_format(nu_path_t *handle, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    nu_string_allocate_vformat((nu_string_t*)handle, format, args);
    va_end(args);
    nu_path_sanatize((nu_string_t*)handle);
}
void nu_path_free(nu_path_t path)
{
    nu_string_free((nu_string_t)path);
}
void nu_path_set_cstr(nu_path_t *handle, const char *path)
{
    nu_string_set_cstr((nu_string_t*)handle, path);
    nu_path_sanatize((nu_string_t*)handle);
}
const char *nu_path_get_cstr(nu_path_t path)
{
    return nu_string_get_cstr((nu_string_t)path);
}
uint32_t nu_path_get_length(nu_path_t path)
{
    return nu_string_get_length((nu_string_t)path);
}
void nu_path_get_filename(nu_path_t path, nu_string_t *filename)
{
    nu_string_allocate_copy((nu_string_t)path, filename);

    uint32_t index_back = nu_string_find_last_cstr((nu_string_t)path, NU_PATH_SEPARATOR);
    if (index_back != nu_string_get_length((nu_string_t)path)) {
        nu_string_erase(filename, 0, index_back + 1);
    }

    uint32_t index_dot = nu_string_find_last_cstr(*filename, ".");
    if (index_dot != nu_string_get_length(*filename)) {
        nu_string_erase(filename, index_dot, nu_string_get_length(*filename) - index_dot);
    }
}
void nu_path_get_directory(nu_path_t path, nu_path_t *directory)
{
    uint32_t index = nu_string_find_last_cstr((nu_string_t)path, NU_PATH_SEPARATOR);
    if (index != nu_string_get_length((nu_string_t)path)) {
        nu_string_allocate_substr((nu_string_t)path, 0, index + 1, (nu_string_t*)directory);
    } else {
        nu_string_allocate((nu_string_t*)directory);
    }
}
void nu_path_get_extension(nu_path_t path, nu_string_t *extension)
{
    uint32_t index_dot = nu_string_find_last_cstr((nu_string_t)path, ".");
    uint32_t index_back = nu_string_find_last_cstr((nu_string_t)path, NU_PATH_SEPARATOR);
    if (index_dot == nu_string_get_length((nu_string_t)path) ||
        (index_back != nu_string_get_length((nu_string_t)path) && index_back > index_dot)) {
        nu_string_allocate(extension);
    } else {
        nu_string_allocate_substr((nu_string_t)path, index_dot + 1, nu_string_get_length((nu_string_t)path) - index_dot - 1, extension);
    }
}
bool nu_path_is_directory(nu_path_t path)
{
    uint32_t len = nu_string_get_length((nu_string_t)path);
    if (len == 0) return true;
    char back = nu_string_get_cstr((nu_string_t)path)[len - 1];
    return back == NU_PATH_SEPARATOR[0];
}
bool nu_path_is_filename(nu_path_t path)
{
    return !nu_path_is_directory(path);
}
void nu_path_join(nu_path_t *path, nu_path_t join)
{
    if (nu_path_is_filename(*path)) return;
    nu_string_append((nu_string_t*)path, (nu_string_t)join);
}