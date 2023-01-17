#include "common.hpp"
#include "algorithms/equation.hpp"
#include "camera.hpp"
#include "objectComponents/hitboxComponent.hpp"

HitboxComponent::HitboxComponent()
    : hitbox_equations_()
    , active_(false) {}

HitboxComponent::HitboxComponent(const std::vector<Equation<Eq::Generic>*> equations)
    : hitbox_equations_(equations)
    , active_(false) {}

HitboxComponent::~HitboxComponent() {
    for (std::vector<Equation<Eq::Generic>*>::iterator it(hitbox_equations_.begin()); it != hitbox_equations_.end(); ++it)
        delete* it;
}

bool HitboxComponent::IsInside(const glm::vec3& point, const glm::mat4& transfo_model) const {
    glm::vec3 point_transfo = glm::inverse(transfo_model) * glm::vec4(point, 1.0f);
    for (int i = 0; i < hitbox_equations_.size(); ++i) {
        if (!hitbox_equations_[i]->Resolve(point_transfo))
            return false;
    }
    return true;
}

bool HitboxComponent::IsOnCamera(const GLfloat& distance, const glm::mat4& transfo_model) {
    glm::vec3 normalized_dir(glm::normalize(G_CameraHandler->GetActive()->getDir()));
    for (GLfloat i = 0.0f; i < distance; i += 0.1f) {
        if (IsInside(G_CameraHandler->GetActive()->getPos() + normalized_dir * i, transfo_model)) {
            active_ = true;
            return true;
        }
    }
    active_ = false;
    return false;
}

SphereHitboxComponent::SphereHitboxComponent(Equation<Eq::Circle>* equation) {
    sphere_hitbox_equations_ = equation;
    active_ = false;
}

SphereHitboxComponent::~SphereHitboxComponent() {
    delete sphere_hitbox_equations_;
}

bool SphereHitboxComponent::IsInside(const glm::vec3& point, const glm::mat4& transfo_model) const {
    glm::vec3 point_transfo = glm::inverse(transfo_model) * glm::vec4(point, 1.0f);
    if (!sphere_hitbox_equations_->Resolve(point_transfo))
        return false;
    return true;
}