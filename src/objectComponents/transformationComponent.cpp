#include "common.hpp"
#include "camera.hpp"
#include "objectComponents/transformationComponent.hpp"

TransformationComponent::TransformationComponent()
    : angle_(0)
    , scale_(glm::vec3(1.0f, 1.0f, 1.0f))
    , rotate_(glm::vec3(1.0f, 1.0f, 1.0f))
    , translate_(glm::vec3(0.0f, 0.0f, 0.0f))
    , model_(glm::scale(scale_)* glm::rotate(angle_, rotate_)* glm::translate(translate_)) {}

TransformationComponent::TransformationComponent(const glm::vec3& scale, const glm::vec3& rotate, const glm::vec3& translate, const GLfloat rotateAngle)
    : angle_(rotateAngle)
    , scale_(scale)
    , rotate_(rotate)
    , translate_(translate)
    , model_(glm::scale(scale_)* glm::rotate(angle_, rotate_)* glm::translate(translate_)) {}

TransformationComponent::~TransformationComponent() {}

void TransformationComponent::reset() {
    model_ = glm::translate(translate_) * glm::rotate(angle_, rotate_) * glm::scale(scale_);
    for (std::vector<glm::mat4>::const_iterator it = additional_.begin(); it != additional_.end(); ++it)
        model_ = model_ * *it;
}

const float TransformationComponent::GetLookingPointDistance() {
    glm::vec3& looking_point(G_CameraHandler->GetActive()->getLookingPoint());
    glm::vec4 looking_point_transfo = glm::inverse(model_) * glm::vec4(looking_point.x, looking_point.y, looking_point.z, 1.0f);
    //DEBUG_NAME_PRINT("looking point distance", glm::sqrt(looking_point_transfo.x * looking_point_transfo.x + looking_point_transfo.y * looking_point_transfo.y + looking_point_transfo.z * looking_point_transfo.z));
    return glm::sqrt(looking_point_transfo.x * looking_point_transfo.x + looking_point_transfo.y * looking_point_transfo.y + looking_point_transfo.z * looking_point_transfo.z);
}