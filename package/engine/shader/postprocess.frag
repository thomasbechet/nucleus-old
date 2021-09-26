#version 450 core

layout(location = 0) out vec4 color;

layout(location = 0) in vec2 pos;
layout(location = 1) in flat mat4 invVPMatrix;

layout(set = 0, binding = 0) uniform EnvironmentUBO {
    mat4 VPMatrix;
    vec3 eye;
};

layout(set = 1, binding = 0) uniform sampler2D imageLight;

__INJECT_CONSTANTS__

vec3 tonemapFilmic(vec3 x) {
	vec3 X = max(vec3(0.0), x - 0.004);
	vec3 result = (X * (6.2 * X + 0.5)) / (X * (6.2 * X + 1.7) + 0.06);
	return pow(result, vec3(2.2));
}

void main() {
    vec2 uv = (pos + 1.0) * 0.5;
    uv.y = 1.0 - uv.y;

    vec4 light = texture(imageLight, uv);
    color = light;

    // // color = vec4(normalDepth.rgb, 1);

    // if (normalDepth.w < MAX_DISTANCE) {
    //     const vec3 backgroundColor = vec3(1., 163., 236.) / vec3(500);
    //     color = vec4(1.0) * max(0, dot(normalDepth.xyz, normalize(vec3(1, 1, 0))));
    //     color += backgroundColor.xyzz * 0.2;
    // } else {
    //     vec4 ptransform = invVPMatrix * vec4(pos, 1, 1);
    //     vec3 dir = normalize((ptransform / ptransform.w).xyz - eye);

    //     const vec3 backgroundColor = vec3(1., 163., 236.) / vec3(500);
    //     color = vec4(backgroundColor + abs(1.0 - dir.y) * 0.4, 1.0);
    // }
}