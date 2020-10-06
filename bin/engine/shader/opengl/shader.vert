#version 330 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec3 in_color;

uniform mat4 mvp;
uniform mat3 normal_to_view;

out struct {
    vec2 uv;
    vec3 normal;
    mat3 tbn;
} va;

void main() {
    gl_Position = mvp * vec4(in_position, 1.0);

    va.uv = in_uv;

    va.normal = normalize(normal_to_view * in_normal);

    vec3 N = normalize(normal_to_view * in_normal);
	vec3 T = normalize(normal_to_view * in_tangent);
	vec3 B = cross(N, T);
	va.tbn = transpose(mat3(T, B, N));
}