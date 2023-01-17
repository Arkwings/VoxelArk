#pragma once

class Block;
class Chunk;

class NoiseMap {
    public:
    NoiseMap();
    ~NoiseMap();

    void LoadChunks(const glm::vec3& pos);
    void UnloadChunk(const int& x, const int& z);
    void Draw(const glm::vec3& position);
    void SetChunkOnCamera();
    Chunk** GetChunk(const int& x, const int& z);
    Chunk** GetChunkOnCamera() { return GetChunk(chunk_on_camera_.first, chunk_on_camera_.second); };
    bool IsChunkOnCamera() { return is_chunk_on_camera_; };
    std::pair<int, int> GetChunkOnCameraPos() { return chunk_on_camera_; };
    Block* GetBlock(const unsigned int& id) { return blocks_[id]; }

    private:
    bool loadChunkFromFile(const int& x, const int& z);
    void genChunk(const int& x, const int& z);

    void saveChunkInFile(const int& x, const int& z);

    Chunk*** chunks_;
    std::unordered_set<std::pair<int, int>, pair_hash> chunks_in_thread_use_;
    std::vector<Block*> blocks_;
    std::pair<int, int> chunk_on_camera_ = { 0, 0 };
    bool is_chunk_on_camera_ = false;
};