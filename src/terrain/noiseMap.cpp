#include "common.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "objectComponents/dataComponent.hpp"
#include "objectComponents/hitboxComponent.hpp"
#include "terrain/block.hpp"
#include "terrain/blocks.hpp"
#include "terrain/chunk.hpp"
#include "terrain/noiseMap.hpp"

NoiseMap::NoiseMap()
    : chunks_(new Chunk** [OPT::MAX_CHUNKS])
    , blocks_(OPT::MAX_BLOCKS, nullptr) {
    TIME_CHECK_START();

#pragma omp parallel for
    for (int i = 0; i < OPT::MAX_CHUNKS; ++i) {
        chunks_[i] = new Chunk * [OPT::MAX_CHUNKS];

        for (unsigned int j = 0; j < OPT::MAX_CHUNKS; ++j)
            chunks_[i][j] = nullptr;
    }

    std::vector<std::string> res;
    for (unsigned char i = 1; i < OPT::MAX_BLOCKS; ++i) {
        res.push_back(G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/top_textures"));   //+ "," + G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/top_normals"));
        res.push_back(G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/side_textures"));   //+ "," + G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/side_normals"));
        res.push_back(G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/bottom_texture"));   //+ "," + G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/bottom_normal"));
        res.push_back(G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/top_over_textures"));   //+ "," + G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/top_over_normals"));
        res.push_back(G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/side_over_textures"));   //+ "," + G_TexJson->Get("/Blocks/" + block_id_name_correspondance[i] + "/side_over_normals"));

        blocks_[i] = new Block(res);
        res.clear();
    }

    TIME_CHECK_END("ctr");
}

NoiseMap::~NoiseMap() {
    for (unsigned int i = 0; i < OPT::MAX_CHUNKS; ++i)
        delete[] chunks_[i];
    delete[] chunks_;

}

void NoiseMap::LoadChunks(const glm::vec3& pos) {
    int x = static_cast<int>(std::floor(pos.x) / static_cast<float>(OPT::CHUNK_SIZE));
    int z = static_cast<int>(std::floor(pos.z) / static_cast<float>(OPT::CHUNK_SIZE));
    int z1, zdist, zmin = z - static_cast<int>(OPT::RENDER_DISTANCE), zmax = z + static_cast<int>(OPT::RENDER_DISTANCE) + 1;
    int x1, xmin = x - static_cast<int>(OPT::RENDER_DISTANCE), xmax = x + static_cast<int>(OPT::RENDER_DISTANCE) + 1;

    Chunk** chk = nullptr;

    for (z1 = zmin; z1 < zmax; ++z1) {
        zdist = (z1 - z) * (z1 - z);

        for (x1 = xmin; x1 < xmax; ++x1) {
            //if ((x1 - x) * (x1 - x) + zdist <= static_cast<int>(OPT::RENDER_DISTANCE) + 1.5f) {

            chk = GetChunk(x1, z1);
            if (*chk == nullptr) {
                if (chunks_in_thread_use_.find({ x1,z1 }) == chunks_in_thread_use_.end()) {   //we do the max we can asyncronously
                    chunks_in_thread_use_.insert({ x1, z1 });
                    if (!loadChunkFromFile(x1, z1)) {
                        G_ThreadPool->QueueJob(std::bind(&NoiseMap::genChunk, this, x1, z1));
                        //genChunk(x1, z1);
                    }
                }
            }
            else {  //we do the gl stuff when the rest is done
                if ((*chk)->points_loaded_ && !(*chk)->graphics_loaded_) {
                    (*chk)->PrepareDraw();
                    std::cout << "prep chunk " << x1 << " " << z1 << std::endl;
                }
            }
        //}
        }
    }
}

void NoiseMap::UnloadChunk(const int& x, const int& z) {

}

void NoiseMap::Draw(const glm::vec3& position) {
    unsigned int line;
    int x = static_cast<int>(std::floor(position.x / static_cast<float>(OPT::CHUNK_SIZE)));
    int z = static_cast<int>(std::floor(position.z / static_cast<float>(OPT::CHUNK_SIZE)));
    int z1, zdist, zmin = z - static_cast<int>(OPT::RENDER_DISTANCE), zmax = z + static_cast<int>(OPT::RENDER_DISTANCE) + 1;
    int x1, xmin = x - static_cast<int>(OPT::RENDER_DISTANCE), xmax = x + static_cast<int>(OPT::RENDER_DISTANCE) + 1;

    glUseProgram((*G_ShaderHandler)[SHA::BLOC3D]->GetID());
    glStencilFunc(GL_ALWAYS, OBJ3D::CUBE, -1);

    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "P"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getP())[0][0]);
    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "V"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getV())[0][0]);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_samplers"), 0);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_samplers"), 1);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "bottom_samplers"), 2);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_overlays"), 3);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_overlays"), 4);
    glUniform3fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "eyePos"), 1, &(G_CameraHandler->GetActive()->getPos())[0]);
    glUniform3fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "sunDir"), 1, &(G_Sky->GetSunDirectionalLight())[0]);
    glUniform3fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "moonDir"), 1, &(G_Sky->GetMoonDirectionalLight())[0]);
    glUniform1f(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "rotation"), G_Sky->GetRotation());

    if (G_NoiseMap->IsChunkOnCamera() && *G_NoiseMap->GetChunkOnCamera() != nullptr) {
        glm::vec3 blockTransfo((*G_NoiseMap->GetChunkOnCamera())->GetTransfoOnCamera());
        glm::vec2 chunkPos = glm::vec2(G_NoiseMap->GetChunkOnCameraPos().first, G_NoiseMap->GetChunkOnCameraPos().second);
        blockTransfo = glm::vec3(static_cast<int>(OPT::CHUNK_SIZE) * chunkPos.x + blockTransfo.x, blockTransfo.y, static_cast<int>(OPT::CHUNK_SIZE) * chunkPos.y + blockTransfo.z);
        glUniform3fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "selectedBlock"), 1, &(blockTransfo[0]));
        glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "isSelectedBlock"), 1);
    }
    else
        glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "isSelectedBlock"), 0);

    Chunk** chk = nullptr;

    for (z1 = zmin; z1 < zmax; ++z1) {
        zdist = (z1 - z) * (z1 - z);
        for (x1 = xmin; x1 < xmax; ++x1) {
            chk = GetChunk(x1, z1);
            if ((x1 - x) * (x1 - x) + zdist <= OPT::RENDER_DISTANCE && *chk != nullptr && (*chk)->graphics_loaded_) {
                (*chk)->Draw();
            }
        }
    }
}

void NoiseMap::SetChunkOnCamera() {
    if (G_CameraHandler->GetActive()->getLookingDistance() < OPT::LOOKING_DISTANCE) {

        const glm::vec3& lookingPoint(G_CameraHandler->GetActive()->getLookingPoint());
        float xf = (lookingPoint.x + 0.5f) / static_cast<float>(OPT::CHUNK_SIZE);
        float zf = (lookingPoint.z + 0.5f) / static_cast<float>(OPT::CHUNK_SIZE);
        int x = static_cast<int>(std::floor(xf));
        int z = static_cast<int>(std::floor(zf));

        Chunk** chk = GetChunk(x, z);

        // std::cout << "chunk [ " << x << " | " << z << " ]" << std::endl;

        //we'll check only the actual chunk, note that the near blocks in x+1 and z+1 chunks could be selected on the side but it shouldn't make a big difference, we prevent selection if it's exactly the same distance to avoid having to chekc in the chunk if the blokc is in an existing index
        if (static_cast<float>(x) != xf
            && static_cast<float>(z) != zf
            && *chk != nullptr
            && (*chk)->graphics_loaded_
            && (*chk)->SetBlockOnCamera(
                static_cast<int>((lookingPoint.x + 0.5f) - x * static_cast<int>(OPT::CHUNK_SIZE)),
                static_cast<int>((lookingPoint.z + 0.5f) - z * static_cast<int>(OPT::CHUNK_SIZE)))) {
            chunk_on_camera_ = std::pair<int, int>(x, z);
            is_chunk_on_camera_ = true;

            //DEBUG_NAME_PRINT("chunk: ", glm::vec2(chunk_on_camera_.first, chunk_on_camera_.second));
        }
    }
    else
        is_chunk_on_camera_ = false;

}

bool NoiseMap::loadChunkFromFile(const int& x, const int& y) {
    return false;
}

void NoiseMap::genChunk(const int& x, const int& z) {
    size_t hashX, hashY, hashXX, hashYY;
    size_t hashXY, hashXXY, hashXYY, hashXXYY;
    hash_combine(hashX, x, OPT::MAP_SEED);
    hash_combine(hashY, z, OPT::MAP_SEED);
    hash_combine(hashXX, x + 1, OPT::MAP_SEED);
    hash_combine(hashYY, z + 1, OPT::MAP_SEED);
    hash_combine(hashXY, x, z, OPT::MAP_SEED);
    hash_combine(hashXXY, x + 1, z, OPT::MAP_SEED);
    hash_combine(hashXYY, x, z + 1, OPT::MAP_SEED);
    hash_combine(hashXXYY, x + 1, z + 1, OPT::MAP_SEED);

    std::mt19937 generator(hashXY);
    std::uniform_int_distribution<uint32_t> distribution(0, 0xffffffff);
    Generation::ArkNoise<GLfloat>* noise_map = new Generation::ArkNoise<GLfloat>(OPT::CHUNK_SIZE, OPT::CHUNK_SIZE, 4.0f);
    noise_map->GenerateGradients(glm::vec4(hashXY, hashXXY, hashXYY, hashXXYY), glm::vec4(hashX, hashXX, hashY, hashYY), static_cast<uint32_t>(distribution(generator)));
    noise_map->GeneratePoints(false, 3, 0.1f, 0.5f);

    Chunk** chunk = GetChunk(x, z);
    *chunk = new Chunk();
    (**chunk).AddNoiseTerain(noise_map->GetPoints());
    (**chunk).SetupTransfos(x, z);
    delete noise_map;

    chunks_in_thread_use_.erase({ x, z });
    std::cout << "gen chunk " << x << " " << z << std::endl;

    chunks_in_thread_use_.erase({ x, z });
}

void NoiseMap::saveChunkInFile(const int& x, const int& z) {

}

Chunk** NoiseMap::GetChunk(const int& x, const int& z) {
    return &chunks_[static_cast<unsigned int>(x < 0 ? static_cast<int>(OPT::MAX_CHUNKS) + x : x)][static_cast<unsigned int>(z < 0 ? static_cast<int>(OPT::MAX_CHUNKS) + z : z)];
}