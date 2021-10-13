#version 450 core

layout(location = 0) out vec4 normalDepthTex;
layout(location = 1) out vec4 positionMaterialTex;

layout(location = 0) in vec2 pos;
layout(location = 1) in flat mat4 invVPMatrix;

__INJECT_CONSTANTS__
__INJECT_ENVIRONMENT__
__INJECT_INSTANCES__

void main() {
    vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
    vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

    vec3 normal;
    uint stepCount;
    uint materialIndex;
    float depth              = tracePrimary(eye, dir, pixelRadiusFactor, normal, materialIndex);
    normalDepthTex.rgba      = vec4(normal, depth);
    positionMaterialTex.rgba = vec4(eye + dir * depth,  uintBitsToFloat(materialIndex));
}