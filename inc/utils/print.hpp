#pragma once

template<typename T, int NB>
std::ostream& operator<<(std::ostream& os, const glm::vec<NB, T>& vec) {
    os << "[";
    for (int i = 0; i < NB - 1; ++i) os << vec[i] << ';';
    os << vec[NB - 1] << "]";
    return os;
}

template<typename T, int NB1, int NB2>
std::ostream& operator<<(std::ostream& os, const glm::mat<NB1, NB2, T>& mat) {
    os << "{";
    for (int j = 0; j < NB2 - 1; ++j) os << mat[j];
    os << mat[NB2 - 1] << "}";
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec) {
    if (!vec.empty()) {
        os << "[";
        for (int i = 0; i < vec.size() - 1; ++i) os << vec[i] << ';';
        os << vec[vec.size() - 1] << "]";
    }
    return os;
}