#include "common.hpp"
#include "shader.hpp"
#include "gamestate.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "render/renderSky3D.hpp"



void renderSky3D(const GameState& interpolated_gs) {

    glDisable(GL_CULL_FACE);
    glUseProgram((*G_ShaderHandler)[SHA::ENV3D]->GetID());

    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::ENV3D]->GetID(), "P"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getP())[0][0]);
    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::ENV3D]->GetID(), "V"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getV())[0][0]);

    G_Sky->Draw();

    glClear(GL_DEPTH_BUFFER_BIT);
}
