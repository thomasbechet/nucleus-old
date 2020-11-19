#version 450
#extension GL_ARB_separate_shader_objects : enable

// Input data
layout(location = 0) in vec3 frag_color;

// Output data
layout(location = 0) out vec4 out_color;

void main() {
    out_color = vec4(frag_color, 1.0);
}