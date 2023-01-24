#include "common.hpp"
#include "objectComponents/dataComponent.hpp"
#include "objectComponents/hitboxComponent.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "terrain/block.hpp"
#include "terrain/blocks.hpp"

Blocks::Blocks()
    // : vertices_((*G_ObjectHandler3D)[OBJ3D::CUBE]->GetData()->GetData())
    // , indexes_((*G_ObjectHandler3D)[OBJ3D::CUBE]->GetData()->GetIndexes())
    : vao_{ 0U, 0U }
    , vbo_{ 0U, 0U }
    , ebo_{ 0U, 0U }
    , mbo_{ 0U, 0U }
    , neighbours_bo_{ 0U, 0U } {}

Blocks::~Blocks() {}

void Blocks::SetNeighbours(std::vector<glm::vec<3, glm::u16>>& neighbours) {
    neighbours_ = std::move(neighbours);
}

void Blocks::SetTransfos(std::vector<glm::mat4>& transfos) {
    transfos_ = std::move(transfos);
}

void Blocks::AddTransfo(const glm::vec3& transfo) {
    transfos_.push_back(
        glm::mat4(
            1.0f, 0, 0, 0,
            0, 1.0f, 0, 0,
            0, 0, 1.0f, 0,
            transfo[0], transfo[1], transfo[2], 1.0f
        ));
}

void Blocks::RemoveTransfo(const glm::vec3& transfo) {
    for (unsigned int i = 0; i < transfos_.size(); ++i) {
        if (transfos_[i][0][3] == transfo.x && transfos_[i][1][3] == transfo.y && transfos_[i][2][3] == transfo.z) {
            transfos_.erase(transfos_.begin() + i);
            break;
        }
    }
}

void Blocks::SetBuffers() {

    const unsigned int inactive_buffer(1 - G_NoiseMap->GetActiveBuffer());

    if (!transfos_.empty() && !neighbours_.empty()) {
        glGenVertexArrays(1, &vao_[0]);
        glGenBuffers(1, &vbo_[0]);
        glGenBuffers(1, &ebo_[0]);
        glGenBuffers(1, &mbo_[0]);
        glGenBuffers(1, &neighbours_bo_[0]);

        glBindVertexArray(vao_[0]);

        // glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
        // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices_.size(), &vertices_.data()[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point_), &point_[0], GL_STATIC_DRAW);

        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_[0]);
        // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_.size() * sizeof(unsigned int), &indexes_[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::POS_OFFSET);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::TEX_OFFSET);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::NORM_OFFSET);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::TAN_OFFSET);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::BITAN_OFFSET);

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, OGL::MAX_BONE_INFLUENCE, GL_UNSIGNED_BYTE, sizeof(Vertex3D), (void*)OGL::BONEIDS_OFFSET);

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, OGL::MAX_BONE_INFLUENCE, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::WEIGHTS_OFFSET);

        glBindBuffer(GL_ARRAY_BUFFER, mbo_[0]);
        glBufferData(GL_ARRAY_BUFFER, transfos_.size() * sizeof(glm::mat4), &transfos_.data()[0], GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)0);
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)sizeof(glm::vec4));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(10);
        glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);
        glVertexAttribDivisor(10, 1);

        glBindBuffer(GL_ARRAY_BUFFER, neighbours_bo_[0]);
        glBufferData(GL_ARRAY_BUFFER, neighbours_.size() * sizeof(glm::vec<3, glm::u16>), &neighbours_.data()[0], GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(11);
        glVertexAttribPointer(11, 3, GL_HALF_FLOAT, GL_FALSE, sizeof(glm::vec<3, glm::u16>), (void*)0);
        glVertexAttribDivisor(11, 1);

        if (OPT::BUFFER_SWAPPING) {
            glGenVertexArrays(1, &vao_[1]);
            glGenBuffers(1, &vbo_[1]);
            glGenBuffers(1, &ebo_[1]);
            glGenBuffers(1, &mbo_[1]);
            glGenBuffers(1, &neighbours_bo_[1]);

            glBindVertexArray(vao_[1]);

            // glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
            // glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex3D) * vertices_.size(), &vertices_.data()[0], GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, vbo_[1]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(point_), &point_[0], GL_STATIC_DRAW);

            // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_[1]);
            // glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_.size() * sizeof(unsigned int), &indexes_[0], GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::POS_OFFSET);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::TEX_OFFSET);

            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::NORM_OFFSET);

            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::TAN_OFFSET);

            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 4, GL_INT_2_10_10_10_REV, GL_TRUE, sizeof(Vertex3D), (void*)OGL::BITAN_OFFSET);

            glEnableVertexAttribArray(5);
            glVertexAttribIPointer(5, OGL::MAX_BONE_INFLUENCE, GL_UNSIGNED_BYTE, sizeof(Vertex3D), (void*)OGL::BONEIDS_OFFSET);

            glEnableVertexAttribArray(6);
            glVertexAttribPointer(6, OGL::MAX_BONE_INFLUENCE, GL_HALF_FLOAT, GL_FALSE, sizeof(Vertex3D), (void*)OGL::WEIGHTS_OFFSET);

            glBindBuffer(GL_ARRAY_BUFFER, mbo_[1]);
            glBufferData(GL_ARRAY_BUFFER, transfos_.size() * sizeof(glm::mat4), &transfos_.data()[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(7);
            glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)0);
            glEnableVertexAttribArray(8);
            glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)sizeof(glm::vec4));
            glEnableVertexAttribArray(9);
            glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)(2 * sizeof(glm::vec4)));
            glEnableVertexAttribArray(10);
            glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, (4 * sizeof(glm::vec4)), (void*)(3 * sizeof(glm::vec4)));

            glVertexAttribDivisor(7, 1);
            glVertexAttribDivisor(8, 1);
            glVertexAttribDivisor(9, 1);
            glVertexAttribDivisor(10, 1);

            glBindBuffer(GL_ARRAY_BUFFER, neighbours_bo_[1]);
            glBufferData(GL_ARRAY_BUFFER, neighbours_.size() * sizeof(glm::vec<3, glm::u16>), &neighbours_.data()[0], GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(11);
            glVertexAttribPointer(11, 3, GL_HALF_FLOAT, GL_FALSE, sizeof(glm::vec<3, glm::u16>), (void*)0);
            glVertexAttribDivisor(11, 1);
        }

        glBindVertexArray(0);
    }
}

void Blocks::Draw(const unsigned int& ID, const unsigned int& active_buffer) {
    if (vao_[active_buffer]) {
        glBindVertexArray(vao_[active_buffer]);
        // G_NoiseMap->GetBlock(ID)->Draw(vao_[active_buffer] != 0U ? indexes_.size() : 0, vao_[active_buffer] != 0U ? transfos_.size() : 0);
        G_NoiseMap->GetBlock(ID)->DrawPoints(vao_[active_buffer] != 0U ? transfos_.size() : 0);

        glBindVertexArray(0);
    }
}