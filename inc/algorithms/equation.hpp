#pragma once

namespace Eq {
    enum Type {
        Generic,
        Circle
    };
};

template <Eq::Type>
class Equation {
    public:
    Equation() {}
    virtual ~Equation() {}

    bool Resolve(const glm::vec3& pos);
};

template <>
bool Equation<Eq::Generic>::Resolve(const glm::vec3& pos);
template <>
bool Equation<Eq::Circle>::Resolve(const glm::vec3& pos);

template class Equation<Eq::Generic>;
template class Equation<Eq::Circle>;