#version 450

in FragmentData
{
    vec3 fs_tex_coord;
    vec4 fs_pos;
    flat vec3 fs_normals;
    flat vec3 fs_tangents;
    flat vec3 fs_bitangents;
    flat vec3 fs_FacesOtherOther;
    flat mat3 fs_TBN;
} inData;

//texture
uniform sampler2DArray top_samplers;
uniform sampler2DArray side_samplers;
uniform sampler2DArray bottom_samplers;
uniform sampler2DArray top_overlays;
uniform sampler2DArray side_overlays;

uniform int top_layers;
uniform int side_layers;
uniform int bottom_layers;
uniform int top_over_layers;
uniform int side_over_layers;

//light
uniform vec3  eyePos;

uniform vec3  lightColor    = vec3(1.0, 1.0, 1.0);
uniform vec3  lightPos      = vec3(23.47, 75.31, 50.79);
uniform vec3  albedo        = vec3(1.0, 1.0, 1.0);
uniform float metallic      = 10.5;
uniform float roughness     = 100.5;
uniform float ao            = 0.5;

//light simpliflied
uniform vec3 sunDir;
uniform vec3 moonDir;
uniform vec3 sunColor = vec3(1.0, 0.9, 0.7);
uniform vec3 moonColor = vec3(0.4, 0.3, 0.6);
uniform float rotation;

uniform float ambientStrength = 0.15f; //[0-1]
uniform float diffStrength = 1.0f;
uniform float shininess = 16.0f;
uniform float specularStrength = 16.0f;
uniform float normalMapStrength = 0.9f; //[0-1]

//selection
uniform vec3 selectedBlock;
uniform int isSelectedBlock;

out vec4 gl_Color;

const float PI_X_2 = 6.28318530717;
const float PI = 3.14159265359;
const float PI_16 = 0.19634954084;

//blinn-phong + normal mapping
vec3 simplifiedLight(vec3 normals, vec3 lightDir, vec3 lightCol, float top_layer, float bottom_layer, float side_layer) {

    vec3 lightDir_tanspace = inData.fs_TBN * lightDir;
    vec3 eyeDir_tanspace = inData.fs_TBN * normalize(eyePos - inData.fs_pos.xyz);

    vec2 tex_coord = inData.fs_tex_coord.xy;
    vec3 normal_tanspace;
    if(normals.y > 0.5)
       normal_tanspace = normalize(texture(top_samplers, vec3(tex_coord,  top_layers/2 + top_layer)).rgb*2.0 - 1.0);
    else if(normals.y < -0.5)
        normal_tanspace = normalize(texture(bottom_samplers, vec3(tex_coord,  bottom_layers/2 + bottom_layer)).rgb*2.0 - 1.0);
    else
        normal_tanspace = normalize(texture(side_samplers, vec3(tex_coord,  side_layers/2 +side_layer)).rgb*2.0 - 1.0);
    normal_tanspace = normalize(normal_tanspace) * normalMapStrength;

    float diff = max(dot(normal_tanspace, lightDir_tanspace), 0.0);
    vec3 diffuse = diff * lightCol * diffStrength;

    //blinn-phong
    float spec = 0.0;
    vec3 halfwayDir_tanspace = normalize(eyeDir_tanspace + lightDir_tanspace);
    if(diff > 0.0) spec = pow(max(dot(normal_tanspace, halfwayDir_tanspace), 0.0), shininess);
    vec3 specular = lightCol * spec;

    return specular + diffuse;
}

// vec3 simplifiedLight(vec3 normals, vec3 lightDir, vec3 lightCol, float top_layer, float bottom_layer, float side_layer) {

//     vec3 eyeDir    = normalize(eyePos - inData.fs_pos.xyz);

//     float diff = max(dot(normals, lightDir), 0.0);
//     vec3 diffuse = diff * lightCol * diffStrength;

//     //blinn-phong
//     float spec = 0.0;
//     vec3 halfwayDir = normalize(eyeDir + lightDir);
//     if(diff > 0.0) spec = pow(max(dot(normals, halfwayDir), 0.0), shininess);
//     vec3 specular = lightCol * spec;

//     return specular + diffuse;
// }

vec4 texturing(vec3 normals, float faces, float top_layer, float bottom_layer, float side_layer) {     

    if(faces >= 32) {
        if(normals.z < -0.5) discard;
        faces = faces - 32;
    }
    if(faces >= 16) {
        if(normals.z > 0.5) discard;
        faces = faces - 16;
    }
    if(faces >= 8) {
        if(normals.x < -0.5) discard;
        faces = faces - 8;
    }
    if(faces >= 4) {
        if(normals.x > 0.5) discard;
        faces = faces - 4;
    }
    if(faces >= 2) {
        if(normals.y < -0.5) discard;
        faces = faces - 2;
    }
    if(faces >= 1) {
        if(normals.y > 0.5) discard;
    }

    vec4 color = vec4(inData.fs_tex_coord.z, 0.0, 0.0, 1.0);

    vec3 selected = vec3(16.0, 131.0, 22.0);

    //for selection
    vec2 tex_coord = inData.fs_tex_coord.xy;
    float displacement = rotation * 15;
    float blockScale = 0.501f;
    if(isSelectedBlock == 1
    && inData.fs_pos.x-blockScale <= selectedBlock.x && inData.fs_pos.y-blockScale <= selectedBlock.y && inData.fs_pos.z-blockScale <= selectedBlock.z
    && inData.fs_pos.x+blockScale >= selectedBlock.x && inData.fs_pos.y+blockScale >= selectedBlock.y && inData.fs_pos.z+blockScale >= selectedBlock.z) {
        float tex_scale = 5.0f;
        float tex_speed = 2.5f;
        float tex_strength = 75.0f;
	    tex_coord.x += cos(tex_coord.y*tex_scale+displacement*tex_speed)/tex_strength;
        tex_coord.y += sin(tex_coord.x*tex_scale+displacement*tex_speed)/tex_strength;
	    tex_coord.x -= cos(tex_coord.y*tex_scale+displacement*tex_speed)/tex_strength;
	    tex_coord.x -= cos(tex_coord.x*tex_scale+displacement*tex_speed)/tex_strength;
    }

    if(normals.y > 0.5f)
        color = texture(top_samplers, vec3(tex_coord,  top_layer));
    else if(normals.y < -0.5f)
        color = texture(bottom_samplers, vec3(tex_coord, bottom_layer));
    else
        color = texture(side_samplers, vec3(tex_coord, side_layer));

    if(color.a < 0.1f || selected == inData.fs_pos.xyz) discard;

    //for selection
    if(isSelectedBlock == 1
    && inData.fs_pos.x-blockScale <= selectedBlock.x && inData.fs_pos.y-blockScale <= selectedBlock.y && inData.fs_pos.z-blockScale <= selectedBlock.z
    && inData.fs_pos.x+blockScale >= selectedBlock.x && inData.fs_pos.y+blockScale >= selectedBlock.y && inData.fs_pos.z+blockScale >= selectedBlock.z) {
        float color_scale = 2.5f;
        float color_speed = 2.5f;
        float color_strength = 50.0f;
	    color.r += cos(tex_coord.y*color_scale+displacement*color_speed)/color_strength;
	    color.r += cos(tex_coord.x*color_scale+displacement*color_speed)/color_strength;
	    color.r += sin(tex_coord.x*color_scale+displacement*color_speed)/color_strength;
        color.g += cos(tex_coord.y*color_scale+displacement*color_speed)/color_strength;
	    color.g += sin(tex_coord.x*color_scale+displacement*color_speed)/color_strength;
	    color.g += cos(tex_coord.x*color_scale+displacement*color_speed)/color_strength;
        color.b += sin(tex_coord.y*color_scale+displacement*color_speed)/color_strength;
	    color.b += cos(tex_coord.x*color_scale+displacement*color_speed)/color_strength;
	    color.b += cos(tex_coord.x*color_scale+displacement*color_speed)/color_strength;

        float man_distance = (abs(inData.fs_pos.x - selectedBlock.x) + abs(inData.fs_pos.y - selectedBlock.y) + abs(inData.fs_pos.z - selectedBlock.z))/3; 
        float mid_dist = man_distance - 0.5f;
        // float mid_dist = abs(distance(fs_pos, selectedBlock)) - 0.5f;
        float dist_factor = 1.0f;
        color.r -= mid_dist * dist_factor;
        color.g -= mid_dist * dist_factor;
        color.b -= mid_dist * dist_factor;
    }

    return color;
}

void main()
{
    float top_layer = inData.fs_tex_coord.z * top_layers / 2;
    float bottom_layer = inData.fs_tex_coord.z * bottom_layers / 2;
    float side_layer = inData.fs_tex_coord.z * side_layers / 2;    
    top_layer = (top_layer * top_layer * top_layer) / (top_layers * top_layers) - 0.5;
    bottom_layer = (bottom_layer * bottom_layer * bottom_layer) / (bottom_layers * bottom_layers) - 0.5;
    side_layer = (side_layer * side_layer * side_layer) / (side_layers * side_layers) - 0.5;
    top_layer = max(0, min((top_layers/2) - 1, floor(top_layer + 0.5)));
    bottom_layer = max(0, min((bottom_layers/2) - 1, floor(bottom_layer + 0.5)));
    side_layer = max(0, min((side_layers/2) - 1, floor(side_layer + 0.5)));

    vec3 normals = normalize(inData.fs_normals.xyz);
    
    vec3 lighting = vec3(ambientStrength);
    //simple
    if(rotation > PI_16 && rotation < PI - PI_16) lighting += simplifiedLight(normals, normalize(moonDir), moonColor, top_layer, bottom_layer, side_layer);
    if(rotation > PI + PI/16 && rotation < PI_X_2 - PI_16)  lighting += simplifiedLight(normals, normalize(sunDir), sunColor, top_layer, bottom_layer, side_layer);
    
    //linear inteprolations
    float rota = rotation;
    if(rota <= PI_16 || rota >= PI_X_2 - PI_16) {   //dusk
        if(rota <= PI_16)   rota += PI_X_2;  
        lighting += ((rota - (PI_X_2 - PI_16)) / (PI_X_2 + PI_16 - (PI_X_2 - PI_16))) * simplifiedLight(normals, normalize(moonDir), moonColor, top_layer, bottom_layer, side_layer);
        lighting += (1-(rota - (PI_X_2 - PI_16)) / (PI_X_2 + PI_16 - (PI_X_2 - PI_16))) * simplifiedLight(normals, normalize(sunDir), sunColor, top_layer, bottom_layer, side_layer);
    }
    if(rota >= PI - PI_16 && rota <= PI + PI_16) {  //dawn
        lighting += (1-(rota - (PI - PI_16)) / (PI + PI_16 - (PI - PI_16))) * simplifiedLight(normals, normalize(moonDir), moonColor, top_layer, bottom_layer, side_layer);
        lighting += ((rota - (PI - PI_16)) / (PI + PI_16 - (PI - PI_16))) * simplifiedLight(normals, normalize(sunDir), sunColor, top_layer, bottom_layer, side_layer);
    }
    vec4 color = texturing(normals, inData.fs_FacesOtherOther.x, top_layer, bottom_layer, side_layer);

    vec3 colorlight = lighting * color.rgb ;
    // colorlight = colorlight / (colorlight + vec3(1.0));
    // colorlight = pow(colorlight, vec3(1.0/2.2));  

    gl_Color = vec4(colorlight, color.a);
    //gl_Color = vec4(normals.x, normals.y, normals.z, 1.0);
    //gl_Color = texture(side_samplers, vec3(inData.fs_tex_coord.xy, 0));
}