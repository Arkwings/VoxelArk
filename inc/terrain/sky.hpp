#pragma once

class Texture;

class Sky {
    public:
    Sky();
    ~Sky();
    Sky(Sky&) = delete;
    Sky& operator=(const Sky&) = delete;

    void UpdateTransfo(const unsigned int& ticks);
    void Draw();

    const glm::vec3& GetSunDirectionalLight() { return sun_directional_light_; }
    const glm::vec3& GetMoonDirectionalLight() { return moon_directional_light_; }
    const float& GetRotation() { return rotation_; }

    private:
    glm::mat4 sun_transfo_ = glm::mat4(1.0f);
    glm::mat4 moon_transfo_ = glm::mat4(1.0f);
    glm::vec3 sun_directional_light_ = glm::vec3(0.0f);
    glm::vec3 moon_directional_light_ = glm::vec3(0.0f);
    float rotation_ = 0.0f;

    Texture* sun_tex_ = nullptr;
    Texture* moon_tex_ = nullptr;
};
