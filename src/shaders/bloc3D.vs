#version 450

layout (location = 0) in vec4 pos_coord;
layout (location = 1) in vec2 tex_coord;
layout (location = 2) in vec4 normals;
layout (location = 3) in vec4 tangents;
layout (location = 4) in vec4 bitangents;
layout (location = 5) in uvec4 boneIDs;
layout (location = 6) in vec4 weights;

layout (location = 7) in mat4 Model;
layout (location = 11) in vec3 FacesOtherOther;

uniform mat4 P;
uniform mat4 V;

out VertexData
{
    flat mat4 gs_M;
    flat vec3 gs_FacesOtherOther;
    float gs_layer;
} outData;

float PHI = 1.61803398874989484820459;  // Φ = Golden Ratio   
float gold_noise(in vec3 xyz){
       return fract(tan(distance(xyz*PHI, xyz)*(abs(xyz.z + xyz.x)) *xyz.y));
}

void main()
{
    mat4 M = Model;

    gl_Position = pos_coord;

    outData.gs_layer = gold_noise(vec3(M[3][0], M[3][1], M[3][2]));
    outData.gs_FacesOtherOther = FacesOtherOther;
    outData.gs_M = M;
}