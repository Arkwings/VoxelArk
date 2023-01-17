#pragma once

namespace CAM {
    constexpr unsigned char FREEY = 0x01;
    constexpr unsigned char LOCKEDY = 0x02;
    constexpr unsigned char FPS = 0x04;
    constexpr unsigned char TPS = 0x08;

    constexpr unsigned char ORTHOGRAPHIC = 0x01;
    constexpr unsigned char PERSPECTIVE = 0x02;
    constexpr unsigned char PERSPECTIVE_INFINITE = 0x04;
};
