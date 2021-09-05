#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 pos;
layout(location = 1) in flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUBO {
    mat4 VPMatrix;
    vec3 eye;
};

__INJECT_CONSTANTS__

layout(set = 1, binding = 0) uniform sampler2D normalDepthTex;

void main() {
    vec2 uv = (pos + 1.0) * 0.5;
    uv.y = 1.0 - uv.y;

    vec4 normalDepth = texture(normalDepthTex, uv);

    // color = vec4(normalDepth.rgb, 1);

    if (normalDepth.w < MAX_DISTANCE) {
        const vec3 backgroundColor = vec3(1., 163., 236.) / vec3(500);
        color = vec4(1.0) * max(0, dot(normalDepth.xyz, normalize(vec3(1, 1, 0))));
        color += backgroundColor.xyzz * 0.2;
    } else {
        vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
        vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

        const vec3 backgroundColor = vec3(1., 163., 236.) / vec3(500);
        color = vec4(backgroundColor + abs(1.0 - dir.y) * 0.4, 1.0);
    }
}