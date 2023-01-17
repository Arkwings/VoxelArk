#include "common.hpp"
#include "shader.hpp"
#include "gamestate.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "object.hpp"
#include "camera.hpp"
#include "terrain/blocks.hpp"
#include "terrain/chunk.hpp"
#include "render/renderLookingPoint3D.hpp"

void renderLookingPoint3D(const GameState& interpolated_gs) {
    glEnable(GL_CULL_FACE);
    glUseProgram((*G_ShaderHandler)[SHA::LINE3D]->GetID());
    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::LINE3D]->GetID(), "P"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getP())[0][0]);
    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::LINE3D]->GetID(), "V"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getV())[0][0]);

    glm::vec2 chunk_pos = glm::vec2(0, 0);
    glm::vec3 transfo = glm::vec3(0, 0, 0);
    if (*G_NoiseMap->GetChunkOnCamera() != nullptr) {
        chunk_pos = glm::vec2(G_NoiseMap->GetChunkOnCameraPos().first, G_NoiseMap->GetChunkOnCameraPos().second);
        transfo = (*G_NoiseMap->GetChunkOnCamera())->GetTransfoOnCamera();

        glm::vec3 translation = glm::vec3(
            static_cast<int>(OPT::CHUNK_SIZE) * chunk_pos.x + transfo.x,
            transfo.y,
            static_cast<int>(OPT::CHUNK_SIZE) * chunk_pos.y + transfo.z);

        (*G_ObjectHandler3D)[OBJ3D::CUBE]->DrawDynamic(
            GL_LINES,
            (*G_ShaderHandler)[SHA::LINE3D]->GetID(),
            glm::translate(translation));
    }
}
