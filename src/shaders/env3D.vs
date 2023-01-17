#version 450

layout (location = 0) in vec3 pos_coord;
layout (location = 1) in vec2 tex_coord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 fs_tex_coord;

void main()
{
    gl_Position = P * V * M * vec4(pos_coord, 1.0);
    fs_tex_coord = vec2(tex_coord);
}