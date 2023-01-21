#version 410

layout (location = 0) in vec4 pos_coord;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;
layout (location = 4) in vec4 bitangents;
layout (location = 5) in uvec4 boneIDs;
layout (location = 6) in vec4 weights;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 fs_tex_coord;
out vec4 fs_pos;
flat out vec4 fs_normals;

void main()
{
    gl_Position = P * V * M * pos_coord;
    fs_tex_coord = tex_coord;
    fs_pos = M * pos_coord;
    fs_normals = transpose(inverse(M)) * normals;
}