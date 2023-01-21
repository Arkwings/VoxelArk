#pragma once

class Block;
class Chunk;

class NoiseMap {
    public:
    NoiseMap();
    ~NoiseMap();

    void Load();
    void LoadChunks(const glm::vec3& pos);
    void UnloadChunk(const int& x, const int& z);
    void Draw(const glm::vec3& position);
    void SetChunkOnCamera();
    Chunk** GetChunk(const int& x, const int& z);
    Chunk** GetChunkOnCamera() { return GetChunk(chunk_on_camera_.first, chunk_on_camera_.second); };
    bool IsChunkOnCamera() { return is_chunk_on_camera_; };
    std::pair<int, int> GetChunkOnCameraPos() { return chunk_on_camera_; };
    Block* GetBlock(const unsigned int& id) { return blocks_[id]; }
    const unsigned int& GetActiveBuffer() { return active_buffer_; }

    private:
    void CPULoad(const int& x, const int& z);
    void GPULoad(const int& x, const int& z);
    bool loadChunkFromFile(const int& x, const int& z);
    void genChunk(const int& x, const int& z);
    void saveChunkInFile(const int& x, const int& z);

    Chunk*** chunks_ = new Chunk * *[OPT::MAX_CHUNKS];
    std::vector<Block*> blocks_ = std::vector<Block*>(OPT::MAX_BLOCKS, nullptr);
    Scheduler* chunk_GL_init_scheduler_ = new Scheduler(OPT::CHUNK_LOADING_SPEED);
    std::pair<int, int> chunk_on_camera_ = { 0, 0 };
    std::unordered_set<size_t> chunk_used_cpu_;
    std::unordered_set<size_t> chunk_used_gpu_;
    std::mutex chunk_used_mutex_;
    bool is_chunk_on_camera_ = false;

    unsigned int active_buffer_ = 0;
};