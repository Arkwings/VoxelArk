#pragma once

namespace OGL {
    constexpr unsigned int MAX_BONE_INFLUENCE = 4U;

    constexpr unsigned int POS_OFFSET = 0U;
    constexpr unsigned int TEX_OFFSET = 8U;
    constexpr unsigned int NORM_OFFSET = 12U;
    constexpr unsigned int TAN_OFFSET = 16U;
    constexpr unsigned int BITAN_OFFSET = 20U;
    constexpr unsigned int BONEIDS_OFFSET = 24U;
    constexpr unsigned int WEIGHTS_OFFSET = 24U + MAX_BONE_INFLUENCE;

    constexpr unsigned int NOT_LOADED = 0U;
    constexpr unsigned int CPU_LOADED = 1U;
    constexpr unsigned int GPU_LOADED = 2U;
};


struct Vertex3D {
    GLhalf  pos_x;
    GLhalf  pos_y;
    GLhalf  pos_z;
    GLhalf  pos_a;

    GLhalf tex_x;
    GLhalf tex_y;

    int32_t norm_a : 2;
    int32_t norm_z : 10;
    int32_t norm_y : 10;
    int32_t norm_x : 10;

    int32_t tan_a : 2;
    int32_t tan_z : 10;
    int32_t tan_y : 10;
    int32_t tan_x : 10;

    int32_t bitan_a : 2;
    int32_t bitan_z : 10;
    int32_t bitan_y : 10;
    int32_t bitan_x : 10;

    GLubyte boneIDs[OGL::MAX_BONE_INFLUENCE];
    GLhalf weights[OGL::MAX_BONE_INFLUENCE];
};

