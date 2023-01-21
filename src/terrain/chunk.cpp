#include "common.hpp"
#include "objectComponents/dataComponent.hpp"
#include "objectComponents/hitboxComponent.hpp"
#include "objectComponents/transformationComponent.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "terrain/block.hpp"
#include "terrain/blocks.hpp"
#include "terrain/chunk.hpp"

Chunk::Chunk()
    : blocks_(OPT::MAX_BLOCKS) {
    //TIME_CHECK_START();

    blocks_pos_ = new unsigned char** [OPT::MAP_HEIGHT];
#pragma omp parallel for
    for (int y = 0; y < OPT::MAP_HEIGHT; ++y) {
        blocks_pos_[y] = new unsigned char* [OPT::CHUNK_SIZE];
        for (unsigned int j = 0; j < OPT::CHUNK_SIZE; ++j) {
            blocks_pos_[y][j] = new unsigned char[OPT::CHUNK_SIZE];
            for (unsigned int i = 0; i < OPT::CHUNK_SIZE; ++i) {
                blocks_pos_[y][j][i] = BLOCKS::AIR;
            }
        }
    }
    //TIME_CHECK_END("Chunk:ctor");

}

Chunk::~Chunk() {
#pragma omp parallel for
    for (int y = 0; y < OPT::MAP_HEIGHT; ++y) {
        for (unsigned int j = 0; j < OPT::CHUNK_SIZE; ++j) {
            delete[] blocks_pos_[y][j];
        }
        delete[] blocks_pos_[y];
    }
    delete[] blocks_pos_;
}

void Chunk::AddNoiseTerain(GLfloat* noisePoints) {
    // TIME_CHECK_START();
    float map_ratio = static_cast<float>(OPT::MAP_HEIGHT) / 8.0f, map_middle = static_cast<float>(OPT::MAP_HEIGHT) / 2.0f;

#pragma omp parallel for
    for (int i = 0U; i < OPT::CHUNK_SIZE * OPT::CHUNK_SIZE * 3; i += 3) {

        unsigned int y = static_cast<int>(noisePoints[i + 1] * map_ratio + map_middle);
        unsigned int z = static_cast<int>(noisePoints[i + 2]);
        unsigned int x = static_cast<int>(noisePoints[i]);

        blocks_pos_[y][z][x] = BLOCKS::GRASS;

        size_t seed;
        hash_combine(seed, x, y, z);
        std::mt19937 generator(seed);
        std::uniform_int_distribution<uint32_t> distribution(4, 7);
        unsigned int dirty = y - distribution(generator); // [4-6] dirt blocks under the grass

        while (y != 1 && y != dirty) blocks_pos_[--y][z][x] = BLOCKS::DIRT;
        while (y != 1) blocks_pos_[--y][z][x] = BLOCKS::STONE;
    }

    // TIME_CHECK_END("noiseTerrain");
}

void Chunk::SetupTransfos(const int& xpos, const int& zpos) {
    // TIME_CHECK_START();

    std::vector<std::vector<glm::mat<4, 4, glm::u16>>> transfos(OPT::MAX_BLOCKS);
    std::vector<std::vector<glm::vec<3, glm::u16>>> neighbours(OPT::MAX_BLOCKS);
    int y, z, x;
    int nei;
    int i;
    float
        xdiff = xpos * static_cast<float>(OPT::CHUNK_SIZE),
        zdiff = zpos * static_cast<float>(OPT::CHUNK_SIZE),
        ydiff = 0;

    for (y = 0; y != OPT::MAP_HEIGHT; ++y) {
        for (z = 0; z != OPT::CHUNK_SIZE; ++z) {
            for (x = 0; x != OPT::CHUNK_SIZE; ++x) {
                if (blocks_pos_[y][z][x] != BLOCKS::AIR) {
                    nei = 0;
                    if (y + 1 < OPT::MAP_HEIGHT && blocks_pos_[y + 1][z][x] != BLOCKS::AIR)     nei |= 1;
                    if (y - 1 >= 0 && blocks_pos_[y - 1][z][x] != BLOCKS::AIR)                  nei |= 2;
                    if (x + 1 < OPT::CHUNK_SIZE && blocks_pos_[y][z][x + 1] != BLOCKS::AIR)     nei |= 4;
                    if (x - 1 >= 0 && blocks_pos_[y][z][x - 1] != BLOCKS::AIR)                  nei |= 8;
                    if (z + 1 < OPT::CHUNK_SIZE && blocks_pos_[y][z + 1][x] != BLOCKS::AIR)     nei |= 16;
                    if (z - 1 >= 0 && blocks_pos_[y][z - 1][x] != BLOCKS::AIR)                  nei |= 32;

                    if (nei != 63) {
                        transfos[blocks_pos_[y][z][x]].push_back(std::move(
                            glm::mat<4, 4, glm::u16>(
                                static_cast<glm::u16>(glm::detail::toFloat16(1.0f)), 0, 0, 0,
                                0, static_cast<glm::u16>(glm::detail::toFloat16(1.0f)), 0, 0,
                                0, 0, static_cast<glm::u16>(glm::detail::toFloat16(1.0f)), 0,
                                glm::detail::toFloat16(x + xdiff), glm::detail::toFloat16(y + ydiff), glm::detail::toFloat16(z + zdiff), static_cast<glm::u16>(glm::detail::toFloat16(1.0f)))
                        ));
                        neighbours[blocks_pos_[y][z][x]].push_back(std::move(
                            glm::vec<3, glm::u16>(glm::detail::toFloat16(static_cast<float>(nei)), 0, 0)
                        ));
                    }
                }
            }
        }
    }

#pragma omp parallel for
    for (i = 1; i < OPT::MAX_BLOCKS; ++i) { //we skip AIR
        blocks_[i].SetTransfos(transfos[i]);
        blocks_[i].SetNeighbours(neighbours[i]);
    }

    // TIME_CHECK_END("setuptransfos");
}

void Chunk::SetBuffers() {
    // TIME_CHECK_START();

    for (unsigned int i = 1; i < OPT::MAX_BLOCKS; ++i) {
        blocks_[i].SetBuffers();
    }

    // TIME_CHECK_END("prepare draw");
}

void Chunk::Draw(const unsigned int& active_buffer) {
    for (unsigned int i = 1U; i < OPT::MAX_BLOCKS; ++i) { //we skip AIR
        blocks_[i].Draw(i, active_buffer);
    }
}

bool Chunk::SetBlockOnCamera(const int& looking_xpos, const int& looking_zpos) {

    const int looking_ypos = static_cast<int>(std::floor(G_CameraHandler->GetActive()->getLookingPoint().y + 0.5f));

    if (looking_ypos >= 0 && looking_ypos <= 255 && blocks_pos_[looking_ypos][looking_zpos][looking_xpos] != BLOCKS::AIR) {
        block_on_camera_ = blocks_pos_[looking_ypos][looking_zpos][looking_xpos];
        transfo_on_camera_ = glm::vec3(looking_xpos, looking_ypos, looking_zpos);
        return true;
    }

    return false;
}