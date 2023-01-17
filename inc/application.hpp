#pragma once

class GameState;
class Window;
class Event;
class ShaderHandler;
class TextureHandler;
class ObjectHandler2D;
class ObjectHandler3D;
class CameraHandler;
class JsonLoader;
class NoiseMap;
class Sky;

class Application {
    public:
    Application();
    ~Application();
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    void Run();

    private:
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
    Sky* sky_ = nullptr;
};

inline Window* G_Window = nullptr;
inline Event* G_Event = nullptr;
inline ShaderHandler* G_ShaderHandler = nullptr;
inline TextureHandler* G_TextureHandler = nullptr;
inline ObjectHandler2D* G_ObjectHandler2D = nullptr;
inline ObjectHandler3D* G_ObjectHandler3D = nullptr;
inline CameraHandler* G_CameraHandler = nullptr;
inline JsonLoader* G_TexJson = nullptr;
inline NoiseMap* G_NoiseMap = nullptr;
inline Sky* G_Sky = nullptr;