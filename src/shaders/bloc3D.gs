#version 450

layout (points) in;
layout (triangle_strip, max_vertices = 24) out;

in VertexData
{
    flat mat4 gs_M;
    flat vec3 gs_FacesOtherOther;
    float gs_layer;
} inData[];

out FragmentData
{
    vec3 fs_tex_coord;
    flat vec4 fs_pos;
    flat vec4 fs_normals;
    flat vec3 fs_FacesOtherOther;
}   outData;

uniform mat4 P;
uniform mat4 V;

const vec4 vertices[8] = vec4[8](
    //front
    vec4(-0.5 , -0.5, -0.5, 1.0),
    vec4(-0.5, 0.5, -0.5, 1.0),
    vec4(0.5, -0.5, -0.5, 1.0),
    vec4( 0.5, 0.5, -0.5, 1.0),
    //back
    vec4(-0.5, -0.5, 0.5, 1.0),
    vec4(-0.5, 0.5, 0.5, 1.0),
    vec4(0.5, -0.5, 0.5, 1.0),
    vec4(0.5, 0.5, 0.5, 1.0)
    );

const vec4 normals[6] = vec4[6](
    vec4(0.0, 1.0, 0.0, 1.0),   //top
    vec4(0.0, -1.0, 0.0, 1.0),  //bottom
    vec4(1.0, 0.0, 0.0, 1.0),   //right
    vec4(-1.0, 0.0, 0.0, 1.0),  //left
    vec4(0.0, 0.0, 1.0, 1.0),   //back
    vec4(0.0, 0.0, -1.0, 1.0)   //front
    );

const vec2 tex_coords[4] = vec2[4](
    vec2(1.0, 1.0),     //top right
    vec2(1.0, 0.0),     //bottom right
    vec2(0.0, 1.0),     //top left
    vec2(0.0, 0.0)      //bottom left
    );

const int indices[24] = int [24]
    (
      1,5,3,7, //top
      6,4,2,0, //bottom 
      2,3,6,7, //right
      4,5,0,1, //left
      6,7,4,5, //back
      0,1,2,3  //front
    );      


void main()
{
    // gl_Position = gl_in[0].gl_Position; 
    // outData.fs_tex_coord = inData[0].gs_tex_coord;
    // outData.fs_pos = inData[0].gs_pos;
    // outData.fs_normals = inData[0].gs_normals;
    // outData.fs_FacesOtherOther = inData[0].gs_FacesOtherOther;
    // EmitVertex();
    
    // gl_Position = gl_in[1].gl_Position; 
    // outData.fs_tex_coord = inData[1].gs_tex_coord;
    // outData.fs_pos = inData[1].gs_pos;
    // outData.fs_normals = inData[1].gs_normals;
    // outData.fs_FacesOtherOther = inData[1].gs_FacesOtherOther;
    // EmitVertex();
    
    // gl_Position = gl_in[2].gl_Position; 
    // outData.fs_tex_coord = inData[2].gs_tex_coord;
    // outData.fs_pos = inData[2].gs_pos;
    // outData.fs_normals = inData[2].gs_normals;
    // outData.fs_FacesOtherOther = inData[2].gs_FacesOtherOther;
    // EmitVertex();

    vec4 temp_vertices[8];  
    vec4 temp_normals[6];  
    mat4 PVM = P * V * inData[0].gs_M;
    mat4 TrasnInvM = transpose(inverse(inData[0].gs_M));

    for (int i=0;i<8; i++) 
        temp_vertices[i]= PVM * (gl_in[0].gl_Position + vertices[i]);
    for (int i=0;i<6; i++) 
        temp_normals[i] = vec4(TrasnInvM * normals[i]); 

    for(int j = 0; j < 6; j++) {
        for (int i = 0; i < 4; i++) {
            gl_Position = temp_vertices[indices[4*j + i]];
            outData.fs_tex_coord = vec3(tex_coords[i], inData[0].gs_layer);
            outData.fs_pos = inData[0].gs_M * vertices[indices[4*j + i]];
            outData.fs_normals = temp_normals[j];
            outData.fs_FacesOtherOther = inData[0].gs_FacesOtherOther;            
            EmitVertex();
        }   
        EndPrimitive();
    }
}