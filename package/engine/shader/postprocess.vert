#version 450 core

layout(location = 1) out vec2 pos;
layout(location = 2) out flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUniformBuffer {
    mat4 VPMatrix;
    vec3 eye;
};

void main() {
    float x = float((gl_VertexIndex & 1) << 2);
    float y = float((gl_VertexIndex & 2) << 1);

    pos = (vec2(x, y) * 0.5) * 2.0 - 1.0;
    invVPMatrix = inverse(VPMatrix);

    gl_Position = vec4(x - 1.0, y - 1.0, 0.0, 1.0);
}