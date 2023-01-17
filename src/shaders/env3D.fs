#version 450

uniform sampler2D tex;
uniform float rotation;
uniform int type;

in vec2 fs_tex_coord;

out vec4 gl_Color;

const float PI = 3.14159265359;
const float PI_2 = 1.57079632679;

float color_interpolation(float min_x, float max_x, float x) {  //0-0.25
    float ratio = (x - min_x) / (max_x - min_x);
    return -(ratio*ratio)+ratio;
}

float color_outerpolation(float min_x, float max_x, float x) {  //0-0.25
    float ratio = (x - min_x) / (max_x - min_x);
    return 0.25+(ratio*ratio)-ratio;
}

void main()
{
    vec4 color = texture(tex, fs_tex_coord);
    if(color.a == 0.0f) discard;

    float rot_inter = rotation < PI? rotation + PI: rotation;
    float inter = color_interpolation(PI, 2*PI, rot_inter);
    float outer = color_outerpolation(PI, 2*PI, rot_inter);

    if(type == 0) {   //sun
        color = vec4(
            color.r + outer*2   + inter,
            color.g - outer*1.5 + inter,
            color.b - outer*1.5 + inter*0.5,
            color.a);
    } else if(type == 1) {      //moon
        color = vec4(
            color.r - outer * 0.35 + inter*0.25,
            color.g - outer * 0.25 + inter*0.25,
            color.b - outer + inter*1.25,
            color.a);
    }
    else {
        color = vec4(0.1,0.1,0.1,1.0);
    }

    gl_Color = vec4(color);
}