#pragma once

class ImageTexture;

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

    ImageTexture* top_tex_ = nullptr;
    ImageTexture* side_tex_ = nullptr;
    ImageTexture* bottom_tex_ = nullptr;
    ImageTexture* top_over_tex_ = nullptr;
    ImageTexture* side_over_tex_ = nullptr;
};
