#version 450

in vec3 fs_tex_coord;
in vec4 fs_pos;
in vec4 fs_normals;
flat in vec3 fs_FacesOtherOther;

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

uniform float ambientStrength = 0.15f;
uniform float diffStrength = 0.7f;
uniform float shininess = 16.0f;
uniform float specularStrength = 16.0f;

//selection
uniform vec3 selectedBlock;
uniform int isSelectedBlock;

out vec4 gl_Color;

const float PI_X_2 = 6.28318530717;
const float PI = 3.14159265359;
const float PI_16 = 0.19634954084;

uniform float time = 100.0;

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 

vec3 lightsComputation(vec3 normals, vec3 lightDir, vec3 lightCol) {
    vec3 lighting;

    vec3 V = normalize(eyePos - fs_pos.xyz);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    //repeat below for mult lights

    // calculate per-light radiance
    // vec3 L = normalize(lightPos - fs_pos.xyz);
    vec3 L = lightDir;
    vec3 H = normalize(V + L);    
    float distance    = 6.0f;
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = lightCol * attenuation;        
    
    // cook-torrance brdf
    float NDF = DistributionGGX(normals, H, roughness);        
    float G   = GeometrySmith(normals, V, L, roughness);      
    vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);    

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
        
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * max(dot(normals, V), 0.0) * max(dot(normals, L), 0.0) + 0.0001;
    vec3 specular     = numerator / denominator;  
            
    // add to outgoing radiance Lo
    float NdotL = max(dot(normals, L), 0.0);                
    lighting += (kD * albedo / PI + specular) * radiance * NdotL;

    return lighting;
}

//blinn-phong
vec3 simplifiedLight(vec3 normals, vec3 lightDir, vec3 lightCol) {
    float diff = max(dot(normals, lightDir), 0.0);
    vec3 diffuse = diff * lightCol * diffStrength;

    vec3 eyeDir    = normalize(eyePos - fs_pos.xyz);
    vec3 halfwayDir = normalize(lightDir + eyeDir);
    // vec3 reflectDir = reflect(-lightDir, normals);

    //blinn-phong
    float spec = pow(max(dot(normals, halfwayDir), 0.0), shininess);
    vec3 specular = lightCol * spec;

    //phong
    // float spec = pow(max(dot(eyeDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;  

    return diffuse + specular;
    return specular;
}

vec4 texturing(vec3 normals, float faces) {     

    if(faces >= 32) {
        if(normals.z < -0.1) discard;
        faces = faces - 32;
    }
    if(faces >= 16) {
        if(normals.z > 0.1) discard;
        faces = faces - 16;
    }
    if(faces >= 8) {
        if(normals.x < -0.1) discard;
        faces = faces - 8;
    }
    if(faces >= 4) {
        if(normals.x > 0.1) discard;
        faces = faces - 4;
    }
    if(faces >= 2) {
        if(normals.y < -0.1) discard;
        faces = faces - 2;
    }
    if(faces >= 1) {
        if(normals.y > 0.1) discard;
    }

    vec4 color = vec4(fs_tex_coord.z, 0.0, 0.0, 1.0);
    float top_layer = fs_tex_coord.z * top_layers;
    float bottom_layer = fs_tex_coord.z * bottom_layers;
    float side_layer = fs_tex_coord.z * side_layers;    
    top_layer = (top_layer * top_layer * top_layer) / (top_layers * top_layers) - 0.5;
    bottom_layer = (bottom_layer * bottom_layer * bottom_layer) / (bottom_layers * bottom_layers) - 0.5;
    side_layer = (side_layer * side_layer * side_layer) / (side_layers * side_layers) - 0.5;

    vec3 selected = vec3(16.0, 131.0, 22.0);

    //for selection
    vec2 tex_coord = fs_tex_coord.xy;
    float displacement = rotation * 15;
    float blockScale = 0.501f;
    if(isSelectedBlock == 1
    && fs_pos.x-blockScale <= selectedBlock.x && fs_pos.y-blockScale <= selectedBlock.y && fs_pos.z-blockScale <= selectedBlock.z
    && fs_pos.x+blockScale >= selectedBlock.x && fs_pos.y+blockScale >= selectedBlock.y && fs_pos.z+blockScale >= selectedBlock.z) {
        float tex_scale = 5.0f;
        float tex_speed = 2.5f;
        float tex_strength = 75.0f;
	    tex_coord.x += cos(tex_coord.y*tex_scale+displacement*tex_speed)/tex_strength;
        tex_coord.y += sin(tex_coord.x*tex_scale+displacement*tex_speed)/tex_strength;
	    tex_coord.x -= cos(tex_coord.y*tex_scale+displacement*tex_speed)/tex_strength;
	    tex_coord.x -= cos(tex_coord.x*tex_scale+displacement*tex_speed)/tex_strength;
    }

    if(normals.y > 0)
        color = texture(top_samplers, vec3(tex_coord, max(0, min(top_layers - 1, floor(top_layer + 0.5))) ));
    else if(normals.y < 0)
        color = texture(bottom_samplers, vec3(tex_coord, max(0, min(bottom_layers - 1, floor(bottom_layer + 0.5)))));
    else
        color = texture(side_samplers, vec3(tex_coord, max(0, min(side_layers - 1, floor(side_layer + 0.5)))));

    if(color.a < 0.1f || selected == fs_pos.xyz) discard;

    //for selection
    if(isSelectedBlock == 1
    && fs_pos.x-blockScale <= selectedBlock.x && fs_pos.y-blockScale <= selectedBlock.y && fs_pos.z-blockScale <= selectedBlock.z
    && fs_pos.x+blockScale >= selectedBlock.x && fs_pos.y+blockScale >= selectedBlock.y && fs_pos.z+blockScale >= selectedBlock.z) {
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

        float man_distance = (abs(fs_pos.x - selectedBlock.x) + abs(fs_pos.y - selectedBlock.y) + abs(fs_pos.z - selectedBlock.z))/3; 
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
    vec3 normals = normalize(fs_normals.xyz);
    
    vec3 lighting = vec3(ambientStrength);
    //simple
    if(rotation > PI_16 && rotation < PI - PI_16) lighting += simplifiedLight(normals, normalize(moonDir), moonColor);
    if(rotation > PI + PI/16 && rotation < PI_X_2 - PI_16)  lighting += simplifiedLight(normals, normalize(sunDir), sunColor);
    
    //linear inteprolations
    float rota = rotation;
    if(rota <= PI_16 || rota >= PI_X_2 - PI_16) {   //dusk
        if(rota <= PI_16)   rota += PI_X_2;  
        lighting += ((rota - (PI_X_2 - PI_16)) / (PI_X_2 + PI_16 - (PI_X_2 - PI_16))) * simplifiedLight(normals, normalize(moonDir), moonColor);
        lighting += (1-(rota - (PI_X_2 - PI_16)) / (PI_X_2 + PI_16 - (PI_X_2 - PI_16))) * simplifiedLight(normals, normalize(sunDir), sunColor);
    }
    if(rota >= PI - PI_16 && rota <= PI + PI_16) {  //dawn
        lighting += (1-(rota - (PI - PI_16)) / (PI + PI_16 - (PI - PI_16))) * simplifiedLight(normals, normalize(moonDir), moonColor);
        lighting += ((rota - (PI - PI_16)) / (PI + PI_16 - (PI - PI_16))) * simplifiedLight(normals, normalize(sunDir), sunColor);
    }
    vec4 color = texturing(normals, fs_FacesOtherOther.x);

    vec3 colorlight = lighting * color.rgb ;
    // colorlight = colorlight / (colorlight + vec3(1.0));
    // colorlight = pow(colorlight, vec3(1.0/2.2));  

    gl_Color = vec4(colorlight, color.a);
}