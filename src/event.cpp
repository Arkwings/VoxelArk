#include "common.hpp"
#include "gamestate.hpp"
#include "camera.hpp"
#include "event.hpp"

Event::Event()
    : keys_(348, 0) {}

Event::~Event() {}

void Event::Process(GameState& gs) {
    processKeyboard(gs);
    processMouse(gs);

    glfwPollEvents();
}

void Event::processKeyboard(GameState& gs) {
    gs.position_updated_ = false;
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_UP) == GLFW_PRESS) { ++gs.charac_front_pos_; gs.position_updated_ = true; }
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) { --gs.charac_front_pos_; gs.position_updated_ = true; }
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) { ++gs.charac_left_pos_; gs.position_updated_ = true; }
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) { --gs.charac_left_pos_; gs.position_updated_ = true; }

#ifdef DEBUG
    if (!gs.debug_print_ && glfwGetKey(G_Window->GetWindow(), GLFW_KEY_KP_0) == GLFW_PRESS && keys_[GLFW_KEY_KP_0] == 0) {
        keys_[GLFW_KEY_KP_0] = 1;
        gs.debug_print_ = true;
        std::cerr << "[INFO] debug render activated" << std::endl;
    }
    else if (gs.debug_print_ && glfwGetKey(G_Window->GetWindow(), GLFW_KEY_KP_0) == GLFW_PRESS && keys_[GLFW_KEY_KP_0] == 0) {
        keys_[GLFW_KEY_KP_0] = 1;
        gs.debug_print_ = false;
        std::cerr << "[INFO] debug render deactivated" << std::endl;
    }
    else if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_KP_0) == GLFW_RELEASE)
        keys_[GLFW_KEY_KP_0] = 0;

#endif
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_F1) == GLFW_PRESS) { G_CameraHandler->SetActive(0); }
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_F2) == GLFW_PRESS) { G_CameraHandler->SetActive(1); }
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_F3) == GLFW_PRESS) { G_CameraHandler->SetActive(2); }
    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_F4) == GLFW_PRESS) { G_CameraHandler->SetActive(3); }

    if (glfwGetKey(G_Window->GetWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(G_Window->GetWindow(), 1);
}

void Event::processMouse(GameState& gs) {
    gs.rotation_updated_ = false;
    glfwGetCursorPos(G_Window->GetWindow(), &gs.charac_side_rota_, &gs.charac_up_rota_);

    if (gs.charac_side_rota_ != G_Window->GetWidth() / 2.0f || gs.charac_up_rota_ != G_Window->GetHeight() / 2.0f) {
        glfwSetCursorPos(G_Window->GetWindow(), G_Window->GetWidth() / 2.0f, G_Window->GetHeight() / 2.0f);
        gs.rotation_updated_ = true;
    }
}