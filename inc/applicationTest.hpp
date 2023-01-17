#pragma once

class BlockTexture;

class ApplicationTest {
    public:
    ApplicationTest();
    ~ApplicationTest();
    ApplicationTest(const ApplicationTest&) = delete;
    ApplicationTest& operator=(const ApplicationTest&) = delete;
    void Run();
    void update(GameState& gs, const std::chrono::nanoseconds& delta);
    void render(GameState const& interpolated_gs);
    void interpolate(GameState const& current_gs, GameState const& previous_gs, GameState& interpolated_gs, float alpha);

    Window* window_ = nullptr;
    Event* event_ = nullptr;
    ShaderHandler* shaderHandler_ = nullptr;
    TextureHandler* textureHandler_ = nullptr;
    ObjectHandler2D* objectHandler2D_ = nullptr;
    ObjectHandler3D* objectHandler3D_ = nullptr;
    CameraHandler* cameraHandler_ = nullptr;

    JsonLoader* texJson_ = nullptr;
    NoiseMap* noiseMap_ = nullptr;

    //test
    BlockTexture* top_tex_ = nullptr;
    BlockTexture* side_tex_ = nullptr;
    BlockTexture* bottom_tex_ = nullptr;
    BlockTexture* top_over_tex_ = nullptr;
    BlockTexture* side_over_tex_ = nullptr;
};
