#include "common.hpp"
#include "algorithms/equation.hpp"

template <>
bool Equation<Eq::Circle>::Resolve(const glm::vec3& pos) {
    return glm::sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z) <= 1;
}

template <>
bool Equation<Eq::Generic>::Resolve(const glm::vec3& pos) {
    std::cout << "not implemented yet" << std::endl;
    return true;
}