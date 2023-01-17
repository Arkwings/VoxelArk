#pragma once

class Blocks;
template <unsigned int Dim> class Object;

class Chunk {
    public:
    Chunk();
    ~Chunk();
    void SetupTransfos(const int& xpos, const int& zpos);
    void AddNoiseTerain(GLfloat* noisePoints);
    void PrepareDraw();
    void Draw();

    std::vector<Blocks>& GetBlocks() { return blocks_; };
    unsigned char*** GetBlocksPos() { return blocks_pos_; };
    bool SetBlockOnCamera(const int& looking_xpos, const int& looking_zpos);
    unsigned char GetBlockOnCamera() { return block_on_camera_; };
    glm::vec3 GetTransfoOnCamera() { return transfo_on_camera_; };

    bool points_loaded_;
    bool graphics_loaded_;

    private:
    std::vector<Blocks> blocks_;   //0-OPT::MAX_BLOCKS-1 IDs max
    unsigned char*** blocks_pos_;

    unsigned char block_on_camera_;
    glm::vec3 transfo_on_camera_;
};
