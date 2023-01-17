#include "common.hpp"
#include "shader.hpp"
#include "gamestate.hpp"
#include "object.hpp"
#include "render/renderUI2D.hpp"

void renderUI2D(const GameState& interpolated_gs) {
    glEnable(GL_CULL_FACE);
    glUseProgram((*G_ShaderHandler)[SHA::STD2D]->GetID());

    glUniform1f(glGetUniformLocation((*G_ShaderHandler)[SHA::STD2D]->GetID(), "screen_ratio"), static_cast<GLfloat>(G_Window->GetHeight()) / G_Window->GetWidth());
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::STD2D]->GetID(), "hasTexture"), 0);
    (*G_ObjectHandler2D)[OBJ2D::POINTER]->DrawStatic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::STD2D]->GetID());
}
