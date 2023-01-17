#include "common.hpp"
#include "object.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "terrain/sky.hpp"

Sky::Sky()
    : sun_tex_(new ImageTexture(string_to_vec_string(G_TexJson->Get("/Sky/SUN"), ',', "[]\""), TEXTURE_2D, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} ,  {TEXTURE_WRAP_S, CLAMP_TO_BORDER}, {TEXTURE_WRAP_T, CLAMP_TO_BORDER}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGRA))
    , moon_tex_(new ImageTexture(string_to_vec_string(G_TexJson->Get("/Sky/MOON"), ',', "[]\""), TEXTURE_2D, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} ,  {TEXTURE_WRAP_S, CLAMP_TO_BORDER}, {TEXTURE_WRAP_T, CLAMP_TO_BORDER}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGRA)) {
    UpdateTransfo(0U);
}

Sky::~Sky() {
    if (moon_tex_) delete moon_tex_;
    if (sun_tex_) delete sun_tex_;
}

void Sky::UpdateTransfo(const unsigned int& ticks) {
    rotation_ = static_cast<float>(ticks) * static_cast<float>(MATH::M_PI_X_2) / static_cast<float>(OPT::TICKS_BY_DAY);  //0 -> 2*PI
    sun_transfo_ = glm::translate(G_CameraHandler->GetActive()->getPos() + glm::vec3(6.0, 0.0, 0.0)) * glm::rotate(rotation_ + static_cast<float>(MATH::M_PI_2), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(glm::vec3(0.0f, 20.0f, 0.0f)) * glm::scale(glm::vec3(7.5f, 7.5f, 7.5f));
    moon_transfo_ = glm::translate(G_CameraHandler->GetActive()->getPos() + glm::vec3(6.0, 0.0, 0.0)) * glm::rotate(rotation_ + static_cast<float>(MATH::M_PI_2), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::translate(glm::vec3(0.0f, -20.0f, 0.0f)) * glm::scale(glm::vec3(7.5f, 7.5f, 7.5f));
    sun_directional_light_ = glm::vec3(sun_transfo_[3][0], sun_transfo_[3][1], sun_transfo_[3][2]) - G_CameraHandler->GetActive()->getPos();
    moon_directional_light_ = glm::vec3(moon_transfo_[3][0], moon_transfo_[3][1], moon_transfo_[3][2]) - G_CameraHandler->GetActive()->getPos();

    //std::cout << sun_directional_light_ << std::endl;
    //std::cout << moon_directional_light_ << std::endl;
}

void Sky::Draw() {
    glUniform1f(glGetUniformLocation((*G_ShaderHandler)[SHA::ENV3D]->GetID(), "rotation"), rotation_);

    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::ENV3D]->GetID(), "type"), 0);
    sun_tex_->Bind(0);
    (*G_ObjectHandler3D)[OBJ3D::SQUARE]->DrawDynamic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::ENV3D]->GetID(), sun_transfo_);

    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::ENV3D]->GetID(), "type"), 1);
    moon_tex_->Bind(0);
    (*G_ObjectHandler3D)[OBJ3D::SQUARE]->DrawDynamic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::ENV3D]->GetID(), moon_transfo_);
}

