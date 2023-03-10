#pragma once

namespace BLOCKS {
    constexpr unsigned char AIR = 0;
    constexpr unsigned char DIRT = 1;
    constexpr unsigned char GRASS = 2;
    constexpr unsigned char STONE = 3;
    constexpr unsigned char SAND = 4;
};

inline std::unordered_map<std::string, unsigned char> block_name_id_correspondance = {
    {"AIR", 0},
    {"DIRT", 1},
    {"GRASS", 2},
    {"STONE", 3},
    {"SAND", 4},
    {"UNNAMED", 5},
    {"UNNAMED", 6},
    {"UNNAMED", 7},
    {"UNNAMED", 8},
    {"UNNAMED", 9},
    {"UNNAMED", 10},
    {"UNNAMED", 11},
    {"UNNAMED", 12},
    {"UNNAMED", 13},
    {"UNNAMED", 14},
    {"UNNAMED", 15},
    {"UNNAMED", 16},
    {"UNNAMED", 17},
    {"UNNAMED", 18},
    {"UNNAMED", 19},
    {"UNNAMED", 20},
    {"UNNAMED", 21},
    {"UNNAMED", 22},
    {"UNNAMED", 23},
    {"UNNAMED", 24},
    {"UNNAMED", 25},
    {"UNNAMED", 26},
    {"UNNAMED", 27},
    {"UNNAMED", 28},
    {"UNNAMED", 29},
    {"UNNAMED", 30},
    {"UNNAMED", 31},
    {"UNNAMED", 32},
    {"UNNAMED", 33},
    {"UNNAMED", 34},
    {"UNNAMED", 35},
    {"UNNAMED", 36},
    {"UNNAMED", 37},
    {"UNNAMED", 38},
    {"UNNAMED", 39},
    {"UNNAMED", 40},
    {"UNNAMED", 41},
    {"UNNAMED", 42},
    {"UNNAMED", 43},
    {"UNNAMED", 44},
    {"UNNAMED", 45},
    {"UNNAMED", 46},
    {"UNNAMED", 47},
    {"UNNAMED", 48},
    {"UNNAMED", 49},
    {"UNNAMED", 50},
    {"UNNAMED", 51},
    {"UNNAMED", 52},
    {"UNNAMED", 53},
    {"UNNAMED", 54},
    {"UNNAMED", 55},
    {"UNNAMED", 56},
    {"UNNAMED", 57},
    {"UNNAMED", 58},
    {"UNNAMED", 59},
    {"UNNAMED", 60},
    {"UNNAMED", 61},
    {"UNNAMED", 62},
    {"UNNAMED", 63},
    {"UNNAMED", 64},
    {"UNNAMED", 65},
    {"UNNAMED", 66},
    {"UNNAMED", 67},
    {"UNNAMED", 68},
    {"UNNAMED", 69},
    {"UNNAMED", 70},
    {"UNNAMED", 71},
    {"UNNAMED", 72},
    {"UNNAMED", 73},
    {"UNNAMED", 74},
    {"UNNAMED", 75},
    {"UNNAMED", 76},
    {"UNNAMED", 77},
    {"UNNAMED", 78},
    {"UNNAMED", 79},
    {"UNNAMED", 80},
    {"UNNAMED", 81},
    {"UNNAMED", 82},
    {"UNNAMED", 83},
    {"UNNAMED", 84},
    {"UNNAMED", 85},
    {"UNNAMED", 86},
    {"UNNAMED", 87},
    {"UNNAMED", 88},
    {"UNNAMED", 89},
    {"UNNAMED", 90},
    {"UNNAMED", 91},
    {"UNNAMED", 92},
    {"UNNAMED", 93},
    {"UNNAMED", 94},
    {"UNNAMED", 95},
    {"UNNAMED", 96},
    {"UNNAMED", 97},
    {"UNNAMED", 98},
    {"UNNAMED", 99},
    {"UNNAMED", 100},
    {"UNNAMED", 101},
    {"UNNAMED", 102},
    {"UNNAMED", 103},
    {"UNNAMED", 104},
    {"UNNAMED", 105},
    {"UNNAMED", 106},
    {"UNNAMED", 107},
    {"UNNAMED", 108},
    {"UNNAMED", 109},
    { "UNNAMED", 110 },
    { "UNNAMED", 111 },
    { "UNNAMED", 112 },
    { "UNNAMED", 113 },
    { "UNNAMED", 114 },
    { "UNNAMED", 115 },
    { "UNNAMED", 116 },
    { "UNNAMED", 117 },
    { "UNNAMED", 118 },
    { "UNNAMED", 119 },
    { "UNNAMED", 120 },
    { "UNNAMED", 121 },
    { "UNNAMED", 122 },
    { "UNNAMED", 123 },
    { "UNNAMED", 124 },
    { "UNNAMED", 125 },
    { "UNNAMED", 126 },
    { "UNNAMED", 127 },
    { "UNNAMED", 128 },
    { "UNNAMED", 129 },
    { "UNNAMED", 130 },
    { "UNNAMED", 131 },
    { "UNNAMED", 132 },
    { "UNNAMED", 133 },
    { "UNNAMED", 134 },
    { "UNNAMED", 135 },
    { "UNNAMED", 136 },
    { "UNNAMED", 137 },
    { "UNNAMED", 138 },
    { "UNNAMED", 139 },
    { "UNNAMED", 140 },
    { "UNNAMED", 141 },
    { "UNNAMED", 142 },
    { "UNNAMED", 143 },
    { "UNNAMED", 144 },
    { "UNNAMED", 145 },
    { "UNNAMED", 146 },
    { "UNNAMED", 147 },
    { "UNNAMED", 148 },
    { "UNNAMED", 149 },
    { "UNNAMED", 150 },
    { "UNNAMED", 151 },
    { "UNNAMED", 152 },
    { "UNNAMED", 153 },
    { "UNNAMED", 154 },
    { "UNNAMED", 155 },
    { "UNNAMED", 156 },
    { "UNNAMED", 157 },
    { "UNNAMED", 158 },
    { "UNNAMED", 159 },
    { "UNNAMED", 160 },
    { "UNNAMED", 161 },
    { "UNNAMED", 162 },
    { "UNNAMED", 163 },
    { "UNNAMED", 164 },
    { "UNNAMED", 165 },
    { "UNNAMED", 166 },
    { "UNNAMED", 167 },
    { "UNNAMED", 168 },
    { "UNNAMED", 169 },
    { "UNNAMED", 170 },
    { "UNNAMED", 171 },
    { "UNNAMED", 172 },
    { "UNNAMED", 173 },
    { "UNNAMED", 174 },
    { "UNNAMED", 175 },
    { "UNNAMED", 176 },
    { "UNNAMED", 177 },
    { "UNNAMED", 178 },
    { "UNNAMED", 179 },
    { "UNNAMED", 180 },
    { "UNNAMED", 181 },
    { "UNNAMED", 182 },
    { "UNNAMED", 183 },
    { "UNNAMED", 184 },
    { "UNNAMED", 185 },
    { "UNNAMED", 186 },
    { "UNNAMED", 187 },
    { "UNNAMED", 188 },
    { "UNNAMED", 189 },
    { "UNNAMED", 190 },
    { "UNNAMED", 191 },
    { "UNNAMED", 192 },
    { "UNNAMED", 193 },
    { "UNNAMED", 194 },
    { "UNNAMED", 195 },
    { "UNNAMED", 196 },
    { "UNNAMED", 197 },
    { "UNNAMED", 198 },
    { "UNNAMED", 199 },
    { "UNNAMED", 200 },
    { "UNNAMED", 201 },
    { "UNNAMED", 202 },
    { "UNNAMED", 203 },
    { "UNNAMED", 204 },
    { "UNNAMED", 205 },
    { "UNNAMED", 206 },
    { "UNNAMED", 207 },
    { "UNNAMED", 208 },
    { "UNNAMED", 209 },
    { "UNNAMED", 210 },
    { "UNNAMED", 211 },
    { "UNNAMED", 212 },
    { "UNNAMED", 213 },
    { "UNNAMED", 214 },
    { "UNNAMED", 215 },
    { "UNNAMED", 216 },
    { "UNNAMED", 217 },
    { "UNNAMED", 218 },
    { "UNNAMED", 219 },
    { "UNNAMED", 220 },
    { "UNNAMED", 221 },
    { "UNNAMED", 222 },
    { "UNNAMED", 223 },
    { "UNNAMED", 224 },
    { "UNNAMED", 225 },
    { "UNNAMED", 226 },
    { "UNNAMED", 227 },
    { "UNNAMED", 228 },
    { "UNNAMED", 229 },
    { "UNNAMED", 230 },
    { "UNNAMED", 231 },
    { "UNNAMED", 232 },
    { "UNNAMED", 233 },
    { "UNNAMED", 234 },
    { "UNNAMED", 235 },
    { "UNNAMED", 236 },
    { "UNNAMED", 237 },
    { "UNNAMED", 238 },
    { "UNNAMED", 239 },
    { "UNNAMED", 240 },
    { "UNNAMED", 241 },
    { "UNNAMED", 242 },
    { "UNNAMED", 243 },
    { "UNNAMED", 244 },
    { "UNNAMED", 245 },
    { "UNNAMED", 246 },
    { "UNNAMED", 247 },
    { "UNNAMED", 248 },
    { "UNNAMED", 249 },
    { "UNNAMED", 250 },
    { "UNNAMED", 251 },
    { "UNNAMED", 252 },
    { "UNNAMED", 253 },
    { "UNNAMED", 254 },
    { "UNNAMED", 255 },
    { "UNNAMED", 256 }
};
inline std::unordered_map<unsigned char, std::string> block_id_name_correspondance = {
    {0, "AIR"},
    {1, "DIRT"},
    {2, "GRASS"},
    {3, "STONE"},
    {4, "SAND"},
    {5, "UNNAMED"},
    {6, "UNNAMED"},
    {7, "UNNAMED"},
    {8, "UNNAMED"},
    {9, "UNNAMED"},
    {10, "UNNAMED"},
    {11, "UNNAMED"},
    {12, "UNNAMED"},
    {13, "UNNAMED"},
    {14, "UNNAMED"},
    {15, "UNNAMED"},
    {16, "UNNAMED"},
    {17, "UNNAMED"},
    {18, "UNNAMED"},
    {19, "UNNAMED"},
    {20, "UNNAMED"},
    {21, "UNNAMED"},
    {22, "UNNAMED"},
    {23, "UNNAMED"},
    {24, "UNNAMED"},
    {25, "UNNAMED"},
    {26, "UNNAMED"},
    {27, "UNNAMED"},
    {28, "UNNAMED"},
    {29, "UNNAMED"},
    {30, "UNNAMED"},
    {31, "UNNAMED"},
    {32, "UNNAMED"},
    {33, "UNNAMED"},
    {34, "UNNAMED"},
    {35, "UNNAMED"},
    {36, "UNNAMED"},
    {37, "UNNAMED"},
    {38, "UNNAMED"},
    {39, "UNNAMED"},
    {40, "UNNAMED"},
    {41, "UNNAMED"},
    {42, "UNNAMED"},
    {43, "UNNAMED"},
    {44, "UNNAMED"},
    {45, "UNNAMED"},
    {46, "UNNAMED"},
    {47, "UNNAMED"},
    {48, "UNNAMED"},
    {49, "UNNAMED"},
    {50, "UNNAMED"},
    {51, "UNNAMED"},
    {52, "UNNAMED"},
    {53, "UNNAMED"},
    {54, "UNNAMED"},
    {55, "UNNAMED"},
    {56, "UNNAMED"},
    {57, "UNNAMED"},
    {58, "UNNAMED"},
    {59, "UNNAMED"},
    {60, "UNNAMED"},
    {61, "UNNAMED"},
    {62, "UNNAMED"},
    {63, "UNNAMED"},
    {64, "UNNAMED"},
    {65, "UNNAMED"},
    {66, "UNNAMED"},
    {67, "UNNAMED"},
    {68, "UNNAMED"},
    {69, "UNNAMED"},
    {70, "UNNAMED"},
    {71, "UNNAMED"},
    {72, "UNNAMED"},
    {73, "UNNAMED"},
    {74, "UNNAMED"},
    {75, "UNNAMED"},
    {76, "UNNAMED"},
    {77, "UNNAMED"},
    {78, "UNNAMED"},
    {79, "UNNAMED"},
    {80, "UNNAMED"},
    {81, "UNNAMED"},
    {82, "UNNAMED"},
    {83, "UNNAMED"},
    {84, "UNNAMED"},
    {85, "UNNAMED"},
    {86, "UNNAMED"},
    {87, "UNNAMED"},
    {88, "UNNAMED"},
    {89, "UNNAMED"},
    {90, "UNNAMED"},
    {91, "UNNAMED"},
    {92, "UNNAMED"},
    {93, "UNNAMED"},
    {94, "UNNAMED"},
    {95, "UNNAMED"},
    {96, "UNNAMED"},
    {97, "UNNAMED"},
    {98, "UNNAMED"},
    {99, "UNNAMED"},
    {100, "UNNAMED"},
    {101, "UNNAMED"},
    {102, "UNNAMED"},
    {103, "UNNAMED"},
    {104, "UNNAMED"},
    {105, "UNNAMED"},
    {106, "UNNAMED"},
    {107, "UNNAMED"},
    {108, "UNNAMED"},
    {109, "UNNAMED"},
    { 110, "UNNAMED" },
    { 111, "UNNAMED" },
    { 112, "UNNAMED" },
    { 113, "UNNAMED" },
    { 114, "UNNAMED" },
    { 115, "UNNAMED" },
    { 116, "UNNAMED" },
    { 117, "UNNAMED" },
    { 118, "UNNAMED" },
    { 119, "UNNAMED" },
    { 120, "UNNAMED" },
    { 121, "UNNAMED" },
    { 122, "UNNAMED" },
    { 123, "UNNAMED" },
    { 124, "UNNAMED" },
    { 125, "UNNAMED" },
    { 126, "UNNAMED" },
    { 127, "UNNAMED" },
    { 128, "UNNAMED" },
    { 129, "UNNAMED" },
    { 130, "UNNAMED" },
    { 131, "UNNAMED" },
    { 132, "UNNAMED" },
    { 133, "UNNAMED" },
    { 134, "UNNAMED" },
    { 135, "UNNAMED" },
    { 136, "UNNAMED" },
    { 137, "UNNAMED" },
    { 138, "UNNAMED" },
    { 139, "UNNAMED" },
    { 140, "UNNAMED" },
    { 141, "UNNAMED" },
    { 142, "UNNAMED" },
    { 143, "UNNAMED" },
    { 144, "UNNAMED" },
    { 145, "UNNAMED" },
    { 146, "UNNAMED" },
    { 147, "UNNAMED" },
    { 148, "UNNAMED" },
    { 149, "UNNAMED" },
    { 150, "UNNAMED" },
    { 151, "UNNAMED" },
    { 152, "UNNAMED" },
    { 153, "UNNAMED" },
    { 154, "UNNAMED" },
    { 155, "UNNAMED" },
    { 156, "UNNAMED" },
    { 157, "UNNAMED" },
    { 158, "UNNAMED" },
    { 159, "UNNAMED" },
    { 160, "UNNAMED" },
    { 161, "UNNAMED" },
    { 162, "UNNAMED" },
    { 163, "UNNAMED" },
    { 164, "UNNAMED" },
    { 165, "UNNAMED" },
    { 166, "UNNAMED" },
    { 167, "UNNAMED" },
    { 168, "UNNAMED" },
    { 169, "UNNAMED" },
    { 170, "UNNAMED" },
    { 171, "UNNAMED" },
    { 172, "UNNAMED" },
    { 173, "UNNAMED" },
    { 174, "UNNAMED" },
    { 175, "UNNAMED" },
    { 176, "UNNAMED" },
    { 177, "UNNAMED" },
    { 178, "UNNAMED" },
    { 179, "UNNAMED" },
    { 180, "UNNAMED" },
    { 181, "UNNAMED" },
    { 182, "UNNAMED" },
    { 183, "UNNAMED" },
    { 184, "UNNAMED" },
    { 185, "UNNAMED" },
    { 186, "UNNAMED" },
    { 187, "UNNAMED" },
    { 188, "UNNAMED" },
    { 189, "UNNAMED" },
    { 190, "UNNAMED" },
    { 191, "UNNAMED" },
    { 192, "UNNAMED" },
    { 193, "UNNAMED" },
    { 194, "UNNAMED" },
    { 195, "UNNAMED" },
    { 196, "UNNAMED" },
    { 197, "UNNAMED" },
    { 198, "UNNAMED" },
    { 199, "UNNAMED" },
    { 200, "UNNAMED" },
    { 201, "UNNAMED" },
    { 202, "UNNAMED" },
    { 203, "UNNAMED" },
    { 204, "UNNAMED" },
    { 205, "UNNAMED" },
    { 206, "UNNAMED" },
    { 207, "UNNAMED" },
    { 208, "UNNAMED" },
    { 209, "UNNAMED" },
    { 210, "UNNAMED" },
    { 211, "UNNAMED" },
    { 212, "UNNAMED" },
    { 213, "UNNAMED" },
    { 214, "UNNAMED" },
    { 215, "UNNAMED" },
    { 216, "UNNAMED" },
    { 217, "UNNAMED" },
    { 218, "UNNAMED" },
    { 219, "UNNAMED" },
    { 220, "UNNAMED" },
    { 221, "UNNAMED" },
    { 222, "UNNAMED" },
    { 223, "UNNAMED" },
    { 224, "UNNAMED" },
    { 225, "UNNAMED" },
    { 226, "UNNAMED" },
    { 227, "UNNAMED" },
    { 228, "UNNAMED" },
    { 229, "UNNAMED" },
    { 230, "UNNAMED" },
    { 231, "UNNAMED" },
    { 232, "UNNAMED" },
    { 233, "UNNAMED" },
    { 234, "UNNAMED" },
    { 235, "UNNAMED" },
    { 236, "UNNAMED" },
    { 237, "UNNAMED" },
    { 238, "UNNAMED" },
    { 239, "UNNAMED" },
    { 240, "UNNAMED" },
    { 241, "UNNAMED" },
    { 242, "UNNAMED" },
    { 243, "UNNAMED" },
    { 244, "UNNAMED" },
    { 245, "UNNAMED" },
    { 246, "UNNAMED" },
    { 247, "UNNAMED" },
    { 248, "UNNAMED" },
    { 249, "UNNAMED" },
    { 250, "UNNAMED" },
    { 251, "UNNAMED" },
    { 252, "UNNAMED" },
    { 253, "UNNAMED" },
    { 254, "UNNAMED" },
    { 255, "UNNAMED" },
    { 256, "UNNAMED" }
};

class DataComponent;
class HitboxComponent;
class Texture;

class Block {
    public:
    Block() = delete;
    Block(const std::vector<std::string>& tex_names, DataComponent* data = new CubeDataComponent(0, false), HitboxComponent* hit = new SphereHitboxComponent(new Equation<Eq::Circle>()), const std::unordered_map<TEXTURE_PARAM_NAME, TEXTURE_PARAM_VALUE>& tex_params = { {TEXTURE_MAX_LEVEL, BASE_LEVEL_9} ,  {TEXTURE_WRAP_S, CLAMP_TO_EDGE}, {TEXTURE_WRAP_T, CLAMP_TO_EDGE}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} });
    ~Block();
    Block(Block&) = delete;
    Block& operator=(const Block&) = delete;

    void Draw(const unsigned int& index_size, const unsigned int& transfo_size);
    void DrawPoints(const unsigned int& transfo_size);

    private:
    DataComponent* data_ = nullptr;
    HitboxComponent* hit_ = nullptr;

    ImageTexture* top_tex_ = nullptr;
    ImageTexture* side_tex_ = nullptr;
    ImageTexture* bottom_tex_ = nullptr;
    ImageTexture* top_over_tex_ = nullptr;
    ImageTexture* side_over_tex_ = nullptr;
};