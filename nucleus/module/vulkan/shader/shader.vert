#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform buffer data
layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

// Input data
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_color;

// Output data
layout(location = 0) out vec2 frag_uv;
layout(location = 1) out vec3 frag_color;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(in_position, 1.0);
    frag_uv     = in_uv;
    frag_color  = in_color;
}