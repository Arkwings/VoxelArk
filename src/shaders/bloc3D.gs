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
    vec4 fs_pos;
    flat vec3 fs_normals;
    flat vec3 fs_tangents;
    flat vec3 fs_bitangents;
    flat vec3 fs_FacesOtherOther;
    flat mat3 fs_TBN;
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

const vec3 normals[6] = vec3[6](
    vec3(0.0, 1.0, 0.0),   //top
    vec3(0.0, -1.0, 0.0),  //bottom
    vec3(1.0, 0.0, 0.0),   //right
    vec3(-1.0, 0.0, 0.0),  //left
    vec3(0.0, 0.0, 1.0),   //back
    vec3(0.0, 0.0, -1.0)   //front
    );

const vec3 tangents[6] = vec3[6](
    vec3(1.0, 0.0, 0.0),   //top
    vec3(-1.0, 0.0, 0.0),  //bottom
    vec3(0.0, 0.0, 1.0),   //right
    vec3(0.0, 0.0, -1.0),  //left
    vec3(-1.0, 0.0, 0.0),   //back
    vec3(1.0, 0.0, 0.0)   //front
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
    vec4 temp_vertices[8];  
    vec3 temp_normals[6];  
    vec3 temp_tangents[6];  

    mat4 PVM = P * V * inData[0].gs_M;
    mat4 TrasnInvM = transpose(inverse(inData[0].gs_M));

    for (int i=0;i<8; i++) 
        temp_vertices[i]= PVM * (gl_in[0].gl_Position + vertices[i]);
    for (int i=0;i<6; i++) 
         temp_normals[i] = vec3(vec4(TrasnInvM * vec4(normals[i], 1.0)).xyz); 
    for (int i=0;i<6; i++) 
    temp_tangents[i] = tangents[i];

    for(int j = 0; j < 6; j++) {
        for (int i = 0; i < 4; i++) {
            gl_Position = temp_vertices[indices[4*j + i]];
            outData.fs_tex_coord = vec3(tex_coords[i], inData[0].gs_layer);
            //outData.fs_pos = vertices[indices[4*j + i]];
            outData.fs_pos = inData[0].gs_M * vertices[indices[4*j + i]];
            outData.fs_normals = temp_normals[j];
            outData.fs_tangents = temp_tangents[j];
            outData.fs_bitangents = normalize(cross(outData.fs_normals, outData.fs_tangents));    
            outData.fs_TBN = transpose(mat3( 
                                mat3(inData[0].gs_M) * outData.fs_tangents,
                                mat3(inData[0].gs_M) * outData.fs_bitangents,
                                mat3(inData[0].gs_M) * outData.fs_normals));    //trnaspose also inverse since it's an orthogonal matrix         
            outData.fs_FacesOtherOther = inData[0].gs_FacesOtherOther;            
            EmitVertex();
        }   
        EndPrimitive();
    }
}