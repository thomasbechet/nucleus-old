#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 pos;

layout(set = 0, binding = 0) uniform EnvironmentUBO {
    mat4 VPMatrix;
    vec3 eye;
};

#include "$ENGINE_DIR/shader/scene.glsl"

layout(set = 1, binding = 0) uniform sampler2D normalDepthTex;

void main() {
    // vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
    // vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

    // const vec3 backgroundColor = vec3(1., 163., 236.) / vec3(500);
    // color = vec4(backgroundColor + abs(1.0 - dir.y) * 0.4, 1.0);

    vec2 uv = (pos + 1.0) * 0.5;
    uv.y = 1.0 - uv.y;

    vec4 normalDepth = texture(normalDepthTex, uv);

    if (normalDepth.w < MAX_DISTANCE) {
        color = vec4(0.7, 0, 0.1, 1) * max(0, dot(normalDepth.xyz, normalize(vec3(1, 1, 0))));
    } else {
        color = vec4(0, 0, 0, 1);
    }
}