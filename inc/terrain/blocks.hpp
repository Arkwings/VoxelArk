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

    void SetNeighbours(const std::vector<float>& neighbours);
    void SetTransfos(const std::vector<glm::vec3>& transfos);
    void AddTransfo(const glm::vec3& transfo);
    void RemoveTransfo(const glm::vec3& transfo);
    void SetArrays();
    void Draw(const unsigned int& ID);

    private:
    std::vector<glm::mat4> transfos_;
    std::vector<glm::vec3> neighbours_;      // x: 0->none, 1 -> top, 2-> bottom, 4-> left, 8->right, 16->front, 32->back, binary and comparison
    const std::vector<Vertex<3>>& vertices_;
    const std::vector<unsigned int>& indexes_;
    unsigned int vao_;
    unsigned int vbo_;
    unsigned int ebo_;
    unsigned int mbo_;
    unsigned int neighbours_bo_;
};

