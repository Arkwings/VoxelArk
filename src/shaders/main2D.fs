#version 410

in vec2 fs_tex_coord;

uniform sampler2D tex_sampler;
uniform int hasTexture;

out vec4 gl_Color;

void main()
{
    vec4 color;
    if(hasTexture == 1) {
        color = texture(tex_sampler, fs_tex_coord);
    }
    else {
        color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    gl_Color = color;
}