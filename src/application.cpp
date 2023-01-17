#include "common.hpp"
#include "gamestate.hpp"
#include "camera.hpp"
#include "terrain/blocks.hpp"
#include "render/renderSky3D.hpp"
#include "render/renderDebug3D.hpp"
#include "render/renderLookingPoint3D.hpp"
#include "render/renderTerrain3D.hpp"
#include "render/renderUI2D.hpp"
#include "application.hpp"

using namespace std::chrono_literals;

static void glfwErrorCallback(int error, const char* description) {
    std::cerr << description << std::endl;
}

Application::Application() {


    //glfw
    if (!glfwInit()) throw std::runtime_error("glfw init failed");
    glfwSetErrorCallback(glfwErrorCallback);
    window_ = new Window();
    event_ = new Event();

    G_Window = window_;
    G_Event = event_;

    //glad
    if (!gladLoadGL(glfwGetProcAddress)) throw std::runtime_error("Failed to initialize OpenGL context");

    //gl
    GLint NumberOfExtensions;
    glGetIntegerv(static_cast<unsigned int>(GL_NUM_EXTENSIONS), &NumberOfExtensions);
    for (int i = 0; i < NumberOfExtensions; i++) {
        const char* ccc = reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i));
        std::cout << ccc << " supported" << std::endl;
    }
    DEBUG_NAME_PRINT("active CG vendor: ", glGetString(GL_VENDOR));
    DEBUG_NAME_PRINT("active CG model: ", glGetString(GL_RENDERER));

    glViewport(0, 0, G_Window->GetWidth(), G_Window->GetHeight());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
#endif
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    // glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shaderHandler_ = new ShaderHandler();
    textureHandler_ = new TextureHandler();
    objectHandler2D_ = new ObjectHandler2D();
    objectHandler3D_ = new ObjectHandler3D();
    cameraHandler_ = new CameraHandler();

    G_ShaderHandler = shaderHandler_;
    G_TextureHandler = textureHandler_;
    G_ObjectHandler2D = objectHandler2D_;
    G_ObjectHandler3D = objectHandler3D_;
    G_CameraHandler = cameraHandler_;

    //test
    // std::cout << (*G_ObjectHandler)[Obj::Sphere3D]->GetHit()->IsInside(glm::vec3(-13.0, 0.0, 12), {}, {}) << std::endl;
    // std::cout << (*G_ObjectHandler)[Obj::Sphere3D]->GetHit()->IsInside(glm::vec3(-13.0, 0.0, 11.9), {}, {}) << std::endl;
    // std::cout << (*G_ObjectHandler)[Obj::Sphere3D]->GetHit()->IsInside(glm::vec3(-13.0, 0.0, 12.1), {}, {}) << std::endl;

    texJson_ = new JsonLoader("properties/textures.json");
    G_TexJson = texJson_;

    noiseMap_ = new NoiseMap();
    G_NoiseMap = noiseMap_;
    G_NoiseMap->LoadChunks(G_CameraHandler->GetActive()->getPos());

    sky_ = new Sky();
    G_Sky = sky_;

}

Application::~Application() {
    delete G_ThreadPool;

    glfwTerminate();
}

void Application::Run() {
    GameState current_gs;
    GameState previous_gs;
    GameState interpolated_gs;

    std::chrono::nanoseconds lag(0ns), timestep(int(1000000000.0f / G_Window->GetRefresh())), chrono_delta(0ns);
    auto update_old(std::chrono::high_resolution_clock::now()), update_now(std::chrono::high_resolution_clock::now());
    auto chrono_start(std::chrono::high_resolution_clock::now()), chrono_now(std::chrono::high_resolution_clock::now());
    auto chrono_seconds_start(std::chrono::high_resolution_clock::now()), chrono_seconds(std::chrono::high_resolution_clock::now());
    float alpha(0.0f);
    int fps_counter(0);

    DEBUG_NAME_PRINT("refresh rate", G_Window->GetRefresh());
    DEBUG_NAME_PRINT("timestep", timestep);

    while (!glfwWindowShouldClose(G_Window->GetWindow())) {
        chrono_now = std::chrono::high_resolution_clock::now();
        chrono_delta = chrono_now - chrono_start;
        chrono_start = chrono_now;
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(chrono_delta);

        G_Event->Process(current_gs);
        do {
            lag -= timestep;
            previous_gs = current_gs;
            update(current_gs, std::chrono::duration_cast<std::chrono::nanoseconds>(update_now - update_old));
            update_old = update_now;
            update_now = std::chrono::high_resolution_clock::now();
        } while (lag > timestep);

        alpha = (static_cast<float>(lag.count()) / timestep.count()) / 1000000000;
        interpolate(current_gs, previous_gs, interpolated_gs, alpha);
        render(interpolated_gs);

        chrono_seconds = std::chrono::high_resolution_clock::now();
        if (chrono_seconds - chrono_seconds_start > 1000000000ns) {
            chrono_seconds_start += 1000000000ns;
            //DEBUG_NAME_PRINT("frames", fps_counter);
            std::cout << "frames: " << fps_counter << std::endl;
            DEBUG_NAME_PRINT("cam pos: ", G_CameraHandler->GetActive()->getPos());
            fps_counter = 0;
        }
        fps_counter += 1;

        std::this_thread::sleep_for(timestep - chrono_delta + lag);
    }
}

void Application::update(GameState& gs, const std::chrono::nanoseconds& delta) {
    gs.Update((float)delta.count() / 1000000000);
}

void Application::render(GameState const& interpolated_gs) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearStencil(0);

    float t = static_cast<float>(static_cast<float>(interpolated_gs.ticks_) / OPT::TICKS_BY_DAY);
    if (t > 0.5f) {     //day
        t = (t - 0.5f) * 2.0f;
        glClearColor(
            quadratic_interpolation(0.255f, 0.527f, t),
            quadratic_interpolation(0.395f, 0.805f, t),
            quadratic_interpolation(0.561f, 0.918f, t),
            1.0f);
    }
    else {              //night
        t = t * 2.0f;
        glClearColor(
            quadratic_interpolation(0.255f, 0.016f, t),
            quadratic_interpolation(0.395f, 0.016f, t),
            quadratic_interpolation(0.561f, 0.204f, t),
            1.0f);
    }

    renderSky3D(interpolated_gs);
#ifdef DEBUG
    renderDebug3D(interpolated_gs);
#endif
    renderTerrain3D(interpolated_gs);
    // renderLookingPoint3D(interpolated_gs);
    renderUI2D(interpolated_gs);

    glfwSwapBuffers(G_Window->GetWindow());

#ifdef DEBUG
    G_ShaderHandler->RecompileShaders();
#endif
}

void Application::interpolate(GameState const& current_gs, GameState const& previous_gs, GameState& interpolated_gs, float alpha) {
#ifdef DEBUG
    interpolated_gs.debug_print_ = current_gs.debug_print_;
#endif
    interpolated_gs.ticks_ = static_cast<unsigned int>(linear_interpolation(static_cast<float>(previous_gs.ticks_), static_cast<float>(current_gs.ticks_), alpha));
}