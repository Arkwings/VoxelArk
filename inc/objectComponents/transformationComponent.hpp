#pragma once

class TransformationComponent {
    public:
    TransformationComponent();
    TransformationComponent(const glm::vec3& scale, const glm::vec3& rotate = glm::vec3(1.0f, 1.0f, 1.0f), const glm::vec3& translate = glm::vec3(0.0f, 0.0f, 0.0f), const GLfloat rotateAngle = 0.0f);
    ~TransformationComponent();

    void SetScale(const glm::vec3& scale) { scale_ = scale; reset(); }
    void SetRotate(const GLfloat& angle, const glm::vec3& rotate) { angle_ = angle; rotate_ = rotate; reset(); }
    void SetTranslate(const glm::vec3& translate) { translate_ = translate; reset(); }
    void SetAdditional(const std::vector<glm::mat4> additional) { additional_ = additional; reset(); }
    const float GetLookingPointDistance();

    const bool& GetOnCamera() const { return onCamera_; }
    void SetOnCamera(const bool& onCamera) { onCamera_ = onCamera; }
    glm::mat4& GetModel() { return model_; }

    private:
    void reset();

    GLfloat angle_;
    glm::vec3 scale_;
    glm::vec3 rotate_;
    glm::vec3 translate_;
    std::vector<glm::mat4> additional_;

    glm::mat4 model_;

    bool onCamera_;
};