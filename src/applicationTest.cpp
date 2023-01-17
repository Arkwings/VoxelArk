#include "common.hpp"
#include "gamestate.hpp"
#include "camera.hpp"
#include "texture.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "terrain/blocks.hpp"
#include "render/renderDebug3D.hpp"
#include "render/renderLookingPoint3D.hpp"
#include "render/renderTerrain3D.hpp"
#include "render/renderUI2D.hpp"
#include "applicationTest.hpp"

using namespace std::chrono_literals;

static void glfwErrorCallback(int error, const char* description) {
    std::cerr << description << std::endl;
}

ApplicationTest::ApplicationTest() {

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
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(MessageCallback, 0);
#endif
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_CULL_FACE);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    cameraHandler_ = new CameraHandler();
    shaderHandler_ = new ShaderHandler();
    textureHandler_ = new TextureHandler();
    objectHandler3D_ = new ObjectHandler3D();

    G_CameraHandler = cameraHandler_;
    G_ShaderHandler = shaderHandler_;
    G_TextureHandler = textureHandler_;
    G_ObjectHandler3D = objectHandler3D_;

    //test
    top_tex_ = new BlockTexture({ "dirt/dirt_top.png","dirt/dirt_top_01.png","dirt/dirt_top_02.png" }, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} , {TEXTURE_WRAP_S, CLAMP_TO_EDGE}, {TEXTURE_WRAP_T, CLAMP_TO_EDGE}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGR);
    side_tex_ = new BlockTexture({ "dirt/dirt.png","dirt/dirt_01.png","dirt/dirt_02.png" }, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} ,  {TEXTURE_WRAP_S, CLAMP_TO_EDGE}, {TEXTURE_WRAP_T, CLAMP_TO_EDGE}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGR);
    bottom_tex_ = new BlockTexture({ "dirt/dirt_top.png" }, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} ,  {TEXTURE_WRAP_S, CLAMP_TO_EDGE}, {TEXTURE_WRAP_T, CLAMP_TO_EDGE}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGR);
    top_over_tex_ = new BlockTexture({}, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} ,  {TEXTURE_WRAP_S, CLAMP_TO_EDGE}, {TEXTURE_WRAP_T, CLAMP_TO_EDGE}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGRA);
    side_over_tex_ = new BlockTexture({}, { {TEXTURE_MAX_LEVEL, BASE_LEVEL_4} ,  {TEXTURE_WRAP_S, CLAMP_TO_EDGE}, {TEXTURE_WRAP_T, CLAMP_TO_EDGE}, {TEXTURE_MIN_FILTER, LINEAR_MIPMAP_LINEAR}, {TEXTURE_MAG_FILTER, LINEAR} }, BGRA);
}

ApplicationTest::~ApplicationTest() {

    delete G_ThreadPool;

    glfwTerminate();
}

void ApplicationTest::Run() {
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

        alpha = static_cast<float>(lag.count()) / timestep.count();
        interpolate(current_gs, previous_gs, interpolated_gs, alpha);
        render(interpolated_gs);

        chrono_seconds = std::chrono::high_resolution_clock::now();
        if (chrono_seconds - chrono_seconds_start > 1000000000ns) {
            chrono_seconds_start += 1000000000ns;
            //DEBUG_NAME_PRINT("frames", fps_counter);
            std::cout << "frames: " << fps_counter << std::endl;
            fps_counter = 0;
        }
        fps_counter += 1;

        std::this_thread::sleep_for(timestep - chrono_delta + lag);
    }
}

void ApplicationTest::update(GameState& gs, const std::chrono::nanoseconds& delta) {
    gs.Update((float)delta.count() / 1000000000);
}

void ApplicationTest::render(GameState const& interpolated_gs) {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    glUseProgram((*G_ShaderHandler)[SHA::BLOC3D]->GetID());

    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "P"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getP())[0][0]);
    glUniformMatrix4fv(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "V"), 1, GL_FALSE, &(G_CameraHandler->GetActive()->getV())[0][0]);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_samplers"), 0);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_samplers"), 1);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "bottom_samplers"), 2);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_overlays"), 3);
    glUniform1i(glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_overlays"), 4);

    top_tex_->Bind(0, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_layers"));
    side_tex_->Bind(1, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_layers"));
    bottom_tex_->Bind(2, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "bottom_layers"));
    top_over_tex_->Bind(3, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "top_over_layers"));
    side_over_tex_->Bind(4, glGetUniformLocation((*G_ShaderHandler)[SHA::BLOC3D]->GetID(), "side_over_layers"));

    (*G_ObjectHandler3D)[OBJ3D::CUBE]->DrawDynamic(GL_TRIANGLES, (*G_ShaderHandler)[SHA::BLOC3D]->GetID());

    glfwSwapBuffers(G_Window->GetWindow());

    G_ShaderHandler->RecompileShaders();
}

void ApplicationTest::interpolate(GameState const& current_gs, GameState const& previous_gs, GameState& interpolated_gs, float alpha) {
#ifdef DEBUG
    interpolated_gs.debug_print_ = current_gs.debug_print_;
#endif
}