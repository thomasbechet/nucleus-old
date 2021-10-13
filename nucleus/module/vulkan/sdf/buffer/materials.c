#include <nucleus/module/vulkan/sdf/buffer/materials.h>

#include <nucleus/module/vulkan/sdf/scene/interface.h>

typedef struct {
    nu_vec3f_t color;
} nuvk_sdf_buffer_material_data_t;

nu_result_t nuvk_sdf_buffer_materials_create(
    nuvk_sdf_buffer_materials_t *buffer,
    const nuvk_context_t *context,
    const nuvk_memory_manager_t *memory_manager,
    const nuvk_render_context_t *render_context
)
{
    buffer->uniform_buffer_range = nuvk_buffer_pad_uniform_buffer_size(context, 
        sizeof(nuvk_sdf_buffer_material_data_t) * NUVK_SDF_MAX_MATERIAL_COUNT);

    nu_result_t result = nuvk_dynamic_range_buffer_create(
        &buffer->dynamic_range_buffer, memory_manager,
        VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        buffer->uniform_buffer_range,
        render_context->max_inflight_frame_count,
        32
    );
    NU_CHECK(result == NU_SUCCESS, return NU_FAILURE, NUVK_LOGGER_NAME, "Failed to create materials buffer.");
    
    return NU_SUCCESS;
}
nu_result_t nuvk_sdf_buffer_materials_destroy(
    nuvk_sdf_buffer_materials_t *buffer,
    const nuvk_memory_manager_t *memory_manager
)
{
    nuvk_dynamic_range_buffer_destroy(&buffer->dynamic_range_buffer, memory_manager);

    return NU_SUCCESS;
}

nu_result_t nuvk_sdf_buffer_materials_write_material(
    nuvk_sdf_buffer_materials_t *buffer,
    uint32_t active_inflight_frame_index,
    uint32_t material_index,
    const nuvk_sdf_material_info_t *info
)
{
    nuvk_sdf_buffer_material_data_t buf;
    nu_vec3f_copy(info->color, buf.color);

    nuvk_dynamic_range_buffer_write(
        &buffer->dynamic_range_buffer, active_inflight_frame_index,
        sizeof(nuvk_sdf_buffer_material_data_t) * material_index, sizeof(nuvk_sdf_buffer_material_data_t),
        &buf);

    return NU_SUCCESS;
}