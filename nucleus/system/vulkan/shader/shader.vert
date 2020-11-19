#version 450
#extension GL_ARB_separate_shader_objects : enable

// Uniform buffer data
layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
} ubo;

// Input data
layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;

// Output data
layout(location = 0) out vec3 frag_color;

void main() {
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(inPosition, 0.0, 1.0);
    frag_color = inColor;
}