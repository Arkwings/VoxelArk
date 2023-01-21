#pragma once

namespace OPT {
    inline float LOOKING_DISTANCE = 6.5f;
    inline float CHUNK_LOADING_SPEED = 0.125f;
    inline unsigned int RENDER_DISTANCE = 5;
    inline unsigned int CHUNK_SIZE = 32;
    inline unsigned int MAP_HEIGHT = 256;
    inline unsigned int MAP_SEED = 12345;
    inline unsigned int MAX_CHUNKS = 25565;
    // inline unsigned int TICKS_BY_DAY = 72000;    //10min
    inline unsigned int TICKS_BY_DAY = 3600;    //30sec
    inline unsigned char MAX_BLOCKS = 5;
    inline bool BUFFER_SWAPPING = false;
};
