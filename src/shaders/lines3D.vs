#version 410

layout (location = 0) in vec3 pos_coord;
layout (location = 7) in mat4 Model;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform int dynamic;

void main()
{
    mat4 Mod;
    if(dynamic == 1)    Mod = M;
    else                Mod = Model;

    gl_Position = P * V * Mod * vec4(pos_coord, 1.0);
}