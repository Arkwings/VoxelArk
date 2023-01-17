#version 410

layout (location = 0) in vec3 pos_coord;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec2 bitangents;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

out vec2 fs_tex_coord;
out vec3 fs_pos;
flat out vec3 fs_normals;
flat out mat3 fs_mat;

void main()
{
    gl_Position = P * V * M * vec4(pos_coord, 1.0);
    fs_tex_coord = tex_coord;
    fs_pos = vec3(M * vec4(pos_coord, 1.0));
    fs_normals = mat3(transpose(inverse(M))) * normals;
}