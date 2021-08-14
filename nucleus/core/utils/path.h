#ifndef NU_PATH_H
#define NU_PATH_H

#include <nucleus/core/common/common.h>
#include <nucleus/core/utils/string.h>

#define NU_PATH_SEPARATOR        "/"
#define NU_PATH_ENGINE_DIRECTORY "engine/"
#define NU_PATH_MODULE_DIRECTORY "engine/module/"
#define NU_PATH_ROOT_DIRECTORY   ""

NU_DECLARE_HANDLE(nu_path_t);

NU_API nu_result_t nu_path_allocate_cstr(const char *path, nu_path_t *handle);
NU_API nu_result_t nu_path_free(nu_path_t path);
NU_API const char *nu_path_get_cstr(nu_path_t path);
NU_API nu_result_t nu_path_get_filename(nu_path_t path, nu_string_t *filename);
NU_API nu_result_t nu_path_get_directory(nu_path_t path, nu_string_t *directory);
NU_API nu_result_t nu_path_get_extension(nu_path_t path, nu_string_t *extension);
NU_API bool nu_path_is_directory(nu_path_t path);
NU_API bool nu_path_is_filename(nu_path_t path);
NU_API nu_result_t nu_path_join(nu_path_t *path, nu_path_t join);

#endif