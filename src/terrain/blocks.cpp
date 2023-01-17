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
    : vertices_((*G_ObjectHandler3D)[OBJ3D::CUBE]->GetData()->GetData())
    , indexes_((*G_ObjectHandler3D)[OBJ3D::CUBE]->GetData()->GetIndexes())
    , vao_(0U)
    , vbo_(0U)
    , ebo_(0U)
    , mbo_(0U)
    , neighbours_bo_(0U) {}

Blocks::~Blocks() {}

void Blocks::SetNeighbours(const std::vector<float>& neighbours) {
    neighbours_.resize(transfos_.size());
    std::transform(std::execution::par_unseq, neighbours.begin(), neighbours.end(), neighbours_.begin(), [](const float& nei) { return glm::vec3(nei, 0, 0); });
}

void Blocks::SetTransfos(const std::vector<glm::vec3>& transfos) {
    transfos_.resize(transfos.size());
    std::transform(std::execution::par_unseq, transfos.begin(), transfos.end(), transfos_.begin(), [](const glm::vec3& vec) { return glm::translate(vec); });
}

void Blocks::AddTransfo(const glm::vec3& transfo) {
    transfos_.push_back(glm::translate(transfo));
}

void Blocks::RemoveTransfo(const glm::vec3& transfo) {
    for (unsigned int i = 0; i < transfos_.size(); ++i) {
        if (transfos_[i][0][3] == transfo.x && transfos_[i][1][3] == transfo.y && transfos_[i][2][3] == transfo.z) {
            transfos_.erase(transfos_.begin() + i);
            break;
        }
    }
}

void Blocks::SetArrays() {
    if (!transfos_.empty() && !neighbours_.empty()) {
        glGenVertexArrays(1, &vao_);
        glBindVertexArray(vao_);

        glGenBuffers(1, &vbo_);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex<3>) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &ebo_);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexes_.size() * sizeof(unsigned int), &indexes_[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>), (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>), (void*)offsetof(Vertex<3>, tex));

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>), (void*)offsetof(Vertex<3>, norm));

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>), (void*)offsetof(Vertex<3>, tan));

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>), (void*)offsetof(Vertex<3>, bitan));

        glEnableVertexAttribArray(5);
        glVertexAttribIPointer(5, MAX_BONE_INFLUENCE, GL_INT, sizeof(Vertex<3>), (void*)offsetof(Vertex<3>, boneIDs));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, MAX_BONE_INFLUENCE, GL_FLOAT, GL_FALSE, sizeof(Vertex<3>), (void*)offsetof(Vertex<3>, weights));

        glGenBuffers(1, &mbo_);
        glBindBuffer(GL_ARRAY_BUFFER, mbo_);
        glBufferData(GL_ARRAY_BUFFER, transfos_.size() * sizeof(glm::mat4), &transfos_.data()[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(1 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(10);
        glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);
        glVertexAttribDivisor(10, 1);

        glGenBuffers(1, &neighbours_bo_);
        glBindBuffer(GL_ARRAY_BUFFER, neighbours_bo_);
        glBufferData(GL_ARRAY_BUFFER, neighbours_.size() * sizeof(glm::vec3), &neighbours_.data()[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(11);
        glVertexAttribPointer(11, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(11, 1);

        glBindVertexArray(0);
    }
}

void Blocks::Draw(const unsigned int& ID) {
    if (vao_) {
        glBindVertexArray(vao_);
        G_NoiseMap->GetBlock(ID)->Draw(indexes_.size(), transfos_.size());
        glBindVertexArray(0);
    }
}