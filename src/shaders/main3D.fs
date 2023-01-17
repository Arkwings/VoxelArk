#version 410

in vec2 fs_tex_coord;
in vec3 fs_pos;
flat in vec3 fs_normals;

//texture
uniform sampler2D default_tex;
uniform int hasTexture;

//light
uniform vec3  eyePos        = vec3(0, 60, 0);
uniform vec3  lightColor    = vec3(23.47, 21.31, 20.79);
uniform vec3  lightPos      = vec3(23.47, 75.31, 50.79);
uniform vec3  albedo        = vec3(1.0, 1.0, 1.0);
uniform float metallic      = 0.5;
uniform float roughness     = 0.5;
uniform float ao            = 0.5;

out vec4 gl_Color;

const float PI = 3.14159265359;

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

vec3 lightsComputation(vec3 normals) {
    vec3 lighting;

    vec3 V = normalize(eyePos - fs_pos);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    //repeat below for mult lights

    // calculate per-light radiance
    vec3 L = normalize(lightPos - fs_pos);
    vec3 H = normalize(V + L);    
    float distance    = length(lightPos - fs_pos);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance     = lightColor * attenuation;        
    
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

vec4 texturing() {
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    
    if(hasTexture == 1) {
        color = texture(default_tex, fs_tex_coord);
    }
    else {
        color = vec4(fs_tex_coord.x, 1.0f, fs_tex_coord.y, 1.0f);
    }
    //if(color.a < 0.1f) discard;

    return color;
}

void main()
{
    vec3 normals = normalize(fs_normals);

    vec3 lighting = lightsComputation(normals);
    vec4 color = texturing();

    vec3 colorlight = color.rgb/* + lighting*/ ;
    // colorlight = colorlight / (colorlight + vec3(1.0));
    // colorlight = pow(colorlight, vec3(1.0/2.2));  

    gl_Color = vec4(colorlight, 1.0);
    // gl_Color = vec4(colorlight, color.a);
}