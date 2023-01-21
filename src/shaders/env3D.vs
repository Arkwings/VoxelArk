#version 450

layout (location = 0) in vec4 pos_coord;
layout (location = 1) in vec2 tex_coord;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 fs_tex_coord;

void main()
{
    gl_Position = P * V * M * pos_coord;
    fs_tex_coord = vec2(tex_coord);
}