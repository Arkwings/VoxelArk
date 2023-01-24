#include "common.hpp" 
#include "objectComponents/dataComponent.hpp"
#include "objectComponents/hitboxComponent.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "terrain/block.hpp"

Block::Block(const std::vector<std::string>& tex_names, DataComponent* data, HitboxComponent* hit, const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& tex_params)
    : data_(data)
    , hit_(hit) {   //to modify to cube
    top_tex_ = new ImageTexture(string_to_vec_string(tex_names[0], ',', "[]\""), TEXTURE_2D_ARRAY, tex_params, BGR);
    side_tex_ = new ImageTexture(string_to_vec_string(tex_names[1], ',', "[]\""), TEXTURE_2D_ARRAY, tex_params, BGR);
    bottom_tex_ = new ImageTexture(string_to_vec_string(tex_names[2], ',', "[]\""), TEXTURE_2D_ARRAY, tex_params, BGR);
    top_over_tex_ = new ImageTexture(string_to_vec_string(tex_names[3], ',', "[]\""), TEXTURE_2D_ARRAY, tex_params, BGRA);
    side_over_tex_ = new ImageTexture(string_to_vec_string(tex_names[4], ',', "[]\""), TEXTURE_2D_ARRAY, tex_params, BGRA);
}

Block::~Block() {
    if (side_over_tex_) delete side_over_tex_;
    if (top_over_tex_) delete top_over_tex_;
    if (bottom_tex_) delete bottom_tex_;
    if (side_tex_) delete side_tex_;
    if (top_tex_) delete top_tex_;
    if (hit_) delete hit_;
    if (data_) delete data_;
}

void Block::Draw(const unsigned int& index_size, const unsigned int& transfo_size) {
    top_tex_->Bind(0, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_layers"));
    side_tex_->Bind(1, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_layers"));
    bottom_tex_->Bind(2, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "bottom_layers"));
    top_over_tex_->Bind(3, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_over_layers"));
    side_over_tex_->Bind(4, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_over_layers"));

    glDrawElementsInstanced(GL_TRIANGLES, index_size, GL_UNSIGNED_INT, 0, transfo_size);
}

void Block::DrawPoints(const unsigned int& transfo_size) {
    top_tex_->Bind(0, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_layers"));
    side_tex_->Bind(1, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_layers"));
    bottom_tex_->Bind(2, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "bottom_layers"));
    top_over_tex_->Bind(3, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_over_layers"));
    side_over_tex_->Bind(4, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_over_layers"));

    glDrawArraysInstanced(GL_POINTS, 0, 1, transfo_size);
}
