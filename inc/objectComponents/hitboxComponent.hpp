#pragma once

class HitboxComponent {
    public:
    HitboxComponent(const std::vector<Equation<Eq::Generic>*> equations);
    virtual ~HitboxComponent();

    virtual bool IsInside(const glm::vec3& point, const glm::mat4& transfo_model) const;
    bool IsOnCamera(const GLfloat& distance, const glm::mat4& transfo_model);
    bool GetActive() const { return active_; };

    protected:
    HitboxComponent();

    std::vector<Equation<Eq::Generic>*> hitbox_equations_;
    bool active_;
};

class SphereHitboxComponent: public HitboxComponent {
    public:
    SphereHitboxComponent() = delete;
    SphereHitboxComponent(Equation<Eq::Circle>* equation = new Equation<Eq::Circle>());   //simplified equation, circle is at (0,0,0) after inverse(M) multiplication
    virtual ~SphereHitboxComponent();

    virtual bool IsInside(const glm::vec3& point, const glm::mat4& transfo_model) const override;

    private:
    Equation<Eq::Circle>* sphere_hitbox_equations_;
};