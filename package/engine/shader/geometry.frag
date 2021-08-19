#version 450 core

layout(location = 0) out vec4 normalDepthTex;

layout(location = 0) in vec2 pos;
layout(location = 1) in flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUBO {
    mat4 VPMatrix;
    vec3 eye;
};

#include "$ENGINE_DIR/shader/scene.glsl"

void main() {
    vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
    vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

    HitInfo hit = intersectScene(eye, dir, MAX_DISTANCE);
    normalDepthTex.rgba = vec4(hit.normal, hit.depth);
}