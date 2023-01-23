#pragma once

class vec3Hash {
    public:

        // Use sum of lengths of first and last names
        // as hash function.
    size_t operator()(const glm::vec3& vec) const {
        size_t res;
        hash_combine(res, vec.x, vec.y, vec.z);
        return res;
    }
};

class Blocks {
    public:
    Blocks();
    ~Blocks();

    void SetNeighbours(std::vector<glm::vec<3, glm::u16>>& neighbours);
    void SetTransfos(std::vector<glm::mat4>& transfos);
    void AddTransfo(const glm::vec3& transfo);
    void RemoveTransfo(const glm::vec3& transfo);
    void SetBuffers();
    void Draw(const unsigned int& ID, const unsigned int& active_buffer);

    private:
    std::vector <glm::mat4> transfos_;
    std::vector<glm::vec<3, glm::u16>> neighbours_;      // x: 0->none, 1 -> top, 2-> bottom, 4-> left, 8->right, 16->front, 32->back, binary and comparison
    const std::vector<Vertex3D>& vertices_;
    const std::vector<unsigned int>& indexes_;
    unsigned int vao_[2];
    unsigned int vbo_[2];
    unsigned int ebo_[2];
    unsigned int mbo_[2];
    unsigned int neighbours_bo_[2];
};

