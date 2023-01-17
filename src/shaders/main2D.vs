#version 410

layout (location = 0) in vec3 pos_coord;
layout (location = 1) in vec2 tex_coord;

uniform float screen_ratio;

out vec2 fs_tex_coord;

void main()
{   
    gl_Position = vec4(pos_coord.x * screen_ratio, pos_coord.y, -1.0, 1.0); //z -> -1.0 is nearest possible from the camera, it will always be on top
    fs_tex_coord = tex_coord;
}