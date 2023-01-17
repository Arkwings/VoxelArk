#pragma once

#define MAX_BONE_INFLUENCE 4

template <int Dim>
struct Vertex {
    glm::vec<Dim, float>  pos;
    glm::vec2 tex;
    glm::vec<Dim, float> norm;
    glm::vec<Dim, float> tan;
    glm::vec<Dim, float> bitan;
    int boneIDs[MAX_BONE_INFLUENCE];
    float weights[MAX_BONE_INFLUENCE];
};