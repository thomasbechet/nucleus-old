#include <nucleus/module/vulkan/core/dynamic_range_buffer.h>

nu_result_t nuvk_dynamic_range_buffer_create(
    nuvk_dynamic_range_buffer_t *buffer, 
    const nuvk_memory_manager_t *memory_manager,
    VkBufferUsageFlagBits buffer_usage,
    uint32_t range_size,
    uint32_t range_count,
    uint32_t block_size
)
{
    /* setup block info */
    buffer->block_size   = block_size;
    buffer->block_count  = ((range_size + 1) / block_size);
    buffer->range_size   = buffer->block_size * buffer->block_count;
    buffer->update_table = (uint8_t*)nu_malloc(sizeof(uint8_t) * buffer->block_count);
    buffer->data         = nu_malloc(buffer->range_size);

    uint32_t total_size  = buffer->range_size * range_count;

    /* initialize update table */
    for (uint32_t i = 0; i < buffer->block_count; i++) {
        buffer->update_table[i] = 0xFF;
    }

    /* create buffer */
    nuvk_buffer_info_t info;
    memset(&info, 0, sizeof(nuvk_buffer_info_t));
    info.buffer_usage = buffer_usage;
    info.memory_usage = VMA_MEMORY_USAGE_CPU_TO_GPU;
    info.size         = total_size;
    
    nu_result_t result = nuvk_buffer_create(&buffer->buffer, memory_manager, &info);
    NU_CHECK(result == NU_SUCCESS, goto cleanup0, NUVK_LOGGER_NAME, "Failed to create internal buffer.");

    result = nuvk_buffer_map(&buffer->buffer, memory_manager);
    NU_CHECK(result == NU_SUCCESS, goto cleanup1, NUVK_LOGGER_NAME, "Failed to map internal buffer.");

    return result;

cleanup1:
    nuvk_buffer_destroy(&buffer->buffer, memory_manager);
cleanup0:
    nu_free(buffer->update_table);
    nu_free(buffer->data);
    return result;
}
nu_result_t nuvk_dynamic_range_buffer_destroy(
    nuvk_dynamic_range_buffer_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_buffer_destroy(&buffer->buffer, memory_manager);
    nu_free(buffer->update_table);
    nu_free(buffer->data);

    return NU_SUCCESS;
}

nu_result_t nuvk_dynamic_range_buffer_write(
    nuvk_dynamic_range_buffer_t *buffer,
    uint32_t range_index,
    uint32_t offset,
    uint32_t size,
    const void *data
)
{
    /* write data */
    memcpy((uint8_t*)buffer->data + offset, data, size);

    /* compute block positions */
    uint32_t index_start = offset / buffer->block_size;
    uint32_t index_stop  = (offset + size) / buffer->block_size;

    /* mark blocks to update */
    uint8_t flag = 0x1 << range_index;
    for (uint32_t i = index_start; i < index_stop + 1; i++) {
        buffer->update_table[i] |= flag;
    }

    return NU_SUCCESS;
}
nu_result_t nuvk_dynamic_range_buffer_transfer(
    nuvk_dynamic_range_buffer_t *buffer,
    uint32_t range_index
)
{
    uint8_t flag = 0x1 << range_index;
    for (uint32_t i = 0; i < buffer->block_count; i++) {
        if (flag & buffer->update_table[i]) {
            uint32_t offset = i * buffer->block_size;
            void *pdst = (uint8_t*)buffer->buffer.map + buffer->range_size * range_index + offset;
            void *psrc = (uint8_t*)buffer->data + i * buffer->block_size;
            memcpy(pdst, psrc, buffer->block_size);
            buffer->update_table[i] &= ~flag;
        }
    }

    return NU_SUCCESS;
}