#version 450 core

layout(location = 0) out vec2 pos;
layout(location = 1) out flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUBO {
    mat4 VPMatrix;
    vec3 eye;
};

void main() {
    pos = vec2(
        float((gl_VertexIndex & 1) << 2),
        float((gl_VertexIndex & 2) << 1)
    ) - 1.0;

    invVPMatrix = inverse(VPMatrix);

    gl_Position = vec4(pos, 0.0, 1.0);
}