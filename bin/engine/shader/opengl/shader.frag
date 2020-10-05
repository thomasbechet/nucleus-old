#version 330 core

out vec3 out_color;

in struct {
    vec2 uv;
    vec3 normal;
    mat3 tbn;
} va;

uniform sampler2D texture0;

void main()
{
    out_color = texture(texture0, vec2(va.uv.x, 1.0 - va.uv.y)).rgb;
}