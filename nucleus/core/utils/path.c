#include <nucleus/core/utils/path.h>

static nu_result_t nu_path_sanatize(nu_string_t *str)
{
    nu_string_replace_cstr(str, "$ENGINE_DIR", NU_PATH_ENGINE_DIRECTORY);
    nu_string_replace_cstr(str, "$MODULE_DIR", NU_PATH_MODULE_DIRECTORY);
    nu_string_replace_cstr(str, "$ROOT_DIR", NU_PATH_ROOT_DIRECTORY);

    nu_string_replace_cstr(str, "\\", NU_PATH_SEPARATOR);
    nu_string_replace_cstr(str, "//", NU_PATH_SEPARATOR);
    if (nu_string_get_cstr(*str)[0] == NU_PATH_SEPARATOR[0]) {
        nu_string_erase(str, 0, 1);
    }

    return NU_SUCCESS;
}

nu_result_t nu_path_allocate_cstr(const char *path, nu_path_t *handle)
{
    nu_string_allocate_cstr(path, (nu_string_t*)handle);
    nu_path_sanatize((nu_string_t*)handle);
    return NU_SUCCESS;
}
nu_result_t nu_path_free(nu_path_t path)
{
    nu_string_free((nu_string_t)path);
    return NU_SUCCESS;
}
const char *nu_path_get_cstr(nu_path_t path)
{
    return nu_string_get_cstr((nu_string_t)path);
}
nu_result_t nu_path_get_filename(nu_path_t path, nu_string_t *filename)
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

    return NU_SUCCESS;
}
nu_result_t nu_path_get_directory(nu_path_t path, nu_string_t *directory)
{
    uint32_t index = nu_string_find_last_cstr((nu_string_t)path, NU_PATH_SEPARATOR);
    if (index != nu_string_get_length((nu_string_t)path)) {
        nu_string_allocate_substr((nu_string_t)path, 0, index + 1, directory);
    } else {
        nu_string_allocate(directory);
    }
    return NU_SUCCESS;
}
nu_result_t nu_path_get_extension(nu_path_t path, nu_string_t *extension)
{
    uint32_t index_dot = nu_string_find_last_cstr((nu_string_t)path, ".");
    uint32_t index_back = nu_string_find_last_cstr((nu_string_t)path, NU_PATH_SEPARATOR);
    if (index_dot == nu_string_get_length((nu_string_t)path) ||
        (index_back != nu_string_get_length((nu_string_t)path) && index_back > index_dot)) {
        nu_string_allocate(extension);
    } else {
        nu_string_allocate_substr((nu_string_t)path, index_dot + 1, nu_string_get_length((nu_string_t)path) - index_dot - 1, extension);
    }
    return NU_SUCCESS;
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
nu_result_t nu_path_join(nu_path_t *path, nu_path_t join)
{
    if (nu_path_is_filename(*path)) return NU_FAILURE;
    nu_string_append((nu_string_t*)path, (nu_string_t)join);
    return NU_SUCCESS;
}