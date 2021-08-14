#version 450 core

#include "$ENGINE_DIR/shader/scene.glsl"

layout(location = 0) out vec4 color;

layout(location = 1) in vec2 pos;
layout(location = 2) in flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUniformBuffer {
    mat4 VPMatrix;
    vec3 eye;
};

void main() {
    vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
    vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

    const vec3 backgroundColor = vec3(1., 163., 236.) / vec3(500);
    color = vec4(backgroundColor + abs(1.0 - dir.y) * 0.4, 1.0);

    HitInfo hit = intersectScene(eye, dir, MAX_DISTANCE);
    if (hit.intersect) {
        color = vec4(1, 0, 0, 1) * max(0, dot(hit.normal, normalize(vec3(1, 1, 0))));
    }
}