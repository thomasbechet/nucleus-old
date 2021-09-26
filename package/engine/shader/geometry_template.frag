#version 450 core

layout(location = 0) out vec4 normalDepthTex;
layout(location = 1) out vec4 positionMaterialTex;

layout(location = 0) in vec2 pos;
layout(location = 1) in flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUBO {
    mat4 VPMatrix;
    vec3 eye;
};

__INJECT_CONSTANTS__
__INJECT_INSTANCES__

void main() {
    vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
    vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

    vec3 normal;
    float depth              = trace(eye, dir, normal);
    normalDepthTex.rgba      = vec4(normal, depth);
    positionMaterialTex.rgba = vec4(eye + dir * depth, 1.0);
}