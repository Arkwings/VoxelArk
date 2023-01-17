#version 450

layout (location = 0) in vec3 pos_coord;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec3 normals;
layout (location = 3) in vec3 tangents;
layout (location = 4) in vec2 bitangents;
layout (location = 5) in ivec4 boneIDs;
layout (location = 6) in vec4 weights;

layout (location = 7) in mat4 Model;
layout (location = 11) in vec3 FacesOtherOther;

uniform mat4 P;
uniform mat4 V;

out vec3 fs_tex_coord;
out vec3 fs_pos;
out vec3 fs_normals;
flat out vec3 fs_FacesOtherOther;

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio   
float gold_noise(in vec3 xyz){
       return fract(tan(distance(xyz*PHI, xyz)*(abs(xyz.z + xyz.x)) *xyz.y));
}

void main()
{
    mat4 M = Model;

    gl_Position = P * V * M * vec4(pos_coord, 1.0);

    float layer = gold_noise(vec3(M[3][0], M[3][1], M[3][2]));
    fs_tex_coord = vec3(tex_coord, layer);
    fs_pos = vec3(M * vec4(pos_coord, 1.0));
    fs_normals = mat3(transpose(inverse(M))) * normals;
    fs_FacesOtherOther = FacesOtherOther;
}