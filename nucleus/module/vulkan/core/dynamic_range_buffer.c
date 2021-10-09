#include <nucleus/module/vulkan/core/dynamic_range_buffer.h>

nu_result_t nuvk_dynamic_range_buffer_create(
    nuvk_dynamic_range_buffer_t *buffer, 
    const nuvk_memory_manager_t *memory_manager,
    uint32_t size,
    uint32_t block_size,
    uint32_t range_count
)
{
    

    buffer->block_size   = block_size;
    buffer->block_count  = (size / block_size)
    buffer->update_table = (uint8_t*)nu_malloc(sizeof(uint8_t) * buffer->block_count);

    for (uint32_t i = 0; i < buffer->block_count; i++) {
        buffer->update_table[i] = 0xFF;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_dynamic_range_buffer_destroy(
    nuvk_dynamic_range_buffer_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    return NU_SUCCESS;
}

nu_result_t nuvk_dynamic_range_buffer_write(
    nuvk_dynamic_range_buffer_t *buffer,
    uint32_t offset,
    uint32_t size,
    void *data
)
{
    return NU_SUCCESS;
}
nu_result_t nuvk_dynamic_range_buffer_write_range(
    nuvk_dynamic_range_buffer_t *buffer,
    uint32_t range_index
)
{
    return NU_SUCCESS;
}