#include "common.hpp"
#include "shader.hpp"
#include "gamestate.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "render/renderTerrain3D.hpp"

void renderTerrain3D(const GameState& interpolated_gs) {
    //stencil to detect which object (up to OPT::MAX_BLOCKS-1 is on cam)
    glEnable(GL_CULL_FACE);
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    //test render 
    glUseProgram((*G_ShaderHandler)[SHA::STD3D]->GetID());

    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::STD3D]->GetID(), "P"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getP())[0][0]);
    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::STD3D]->GetID(), "V"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getV())[0][0]);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::STD3D]->GetID(), "hasTexture"), 1);

    (*G_TextureHandler)[TEX::NB1]->Bind(0);
    (*G_ObjectHandler3D)[OBJ3D::SPHERE]->DrawDynamic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::STD3D]->GetID());

    (*G_TextureHandler)[TEX::PERLINTESTMID]->Bind(0);
    (*G_ObjectHandler3D)[OBJ3D::CUBE]->DrawDynamic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::STD3D]->GetID());

    G_NoiseMap->Draw(G_CameraHandler->GetActive()->getPos());

    glDisable(GL_STENCIL_TEST);
}