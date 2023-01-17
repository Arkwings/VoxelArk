#include "common.hpp"
#include "shader.hpp"
#include "gamestate.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "render/renderDebug3D.hpp"

#ifdef DEBUG
void renderDebug3D(const GameState& interpolated_gs) {
    if (interpolated_gs.debug_print_) {
        glDisable(GL_CULL_FACE);
        glUseProgram((*G_ShaderHandler)[SHA::STD3D]->GetID());

        glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::STD3D]->GetID(), "P"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getP())[0][0]);
        glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::STD3D]->GetID(), "V"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getV())[0][0]);
        (*G_TextureHandler)[TEX::DBG]->Bind(0);
        glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::STD3D]->GetID(), "hasTexture"), 1);

        glm::vec3& pos(G_CameraHandler->GetActive()->getPos());
        int x = static_cast<int>(pos.x) / 20, z = static_cast<int>(pos.z) / 20;
        if (pos.x < 0) --x; if (pos.z < 0) --z;

        for (int j = z - 1; j != z + 2; ++j) {
            for (int i = x - 1; i != x + 2; ++i) {
                (*G_ObjectHandler3D)[OBJ3D::SQUARE]->GetTransfoDynamic(0)->SetTranslate(glm::vec3(i * 20 + 10, 0, j * 20 + 10));
                (*G_ObjectHandler3D)[OBJ3D::SQUARE]->DrawDynamic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::STD3D]->GetID(), (*G_ObjectHandler3D)[OBJ3D::SQUARE]->GetTransfoDynamic(0)->GetModel());
            }
        }
    }
}
#endif