#include <stdio.h>

#include "../renderer.h"
#include "../common/logger.h"

static nu_result_t read_binary_file(const char *path, char **buffer, uint32_t *size)
{
    FILE *file;
    
    file = fopen(path, "rb");
    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = (char*)nu_malloc(sizeof(char) * (*size));

    if (*buffer == NULL) {
        nu_warning(NUVK_LOGGER_VULKAN" Failed to load file %s.\n", path);
        return NU_FAILURE;
    }

    if (fread(*buffer, sizeof(char), *size, file) != (*size)) {
        nu_warning(NUVK_LOGGER_VULKAN" Failed to read file %s.\n", path);
        nu_free(*buffer);
        return NU_FAILURE;
    }

    fclose(file);

    return NU_SUCCESS;
}