#include "common.hpp"
#include "window.hpp"

Window::Window()
    : monitors_(nullptr)
    , modes_(nullptr)
    , window_(nullptr)
    , width_(-1)
    , height_(-1)
    , red_(-1)
    , green_(-1)
    , blue_(-1)
    , refresh_(-1)
    , max_width_(-1)
    , max_height_(-1)
    , max_red_(-1)
    , max_green_(-1)
    , max_blue_(-1)
    , max_refresh_(-1)
    , fullscreen_(false) {
    updateInfos();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); //disable for test purposes
    glfwWindowHint(GLFW_RED_BITS, red_);
    glfwWindowHint(GLFW_GREEN_BITS, green_);
    glfwWindowHint(GLFW_BLUE_BITS, blue_);
    glfwWindowHint(GLFW_REFRESH_RATE, refresh_);

    window_ = glfwCreateWindow(width_, height_, "GL Sandbox", nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        exit(1);
    }

    // this is for an in-game mode ( mouse trapping )
    glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window_, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window_);

    //may be useful for debug infos later
    glfwSetTime(0.0f);
}

Window::~Window() {
    glfwDestroyWindow(window_);
}

void Window::updateInfos() {
    monitors_ = glfwGetMonitors(&monitor_count_);
    DEBUG_NAME_PRINT("[WINDOW] monitors", monitor_count_);
    modes_ = glfwGetVideoModes(monitors_[0], &video_count_);

    for (int i = 0; i < video_count_; ++i) {
        if (modes_[i].width > max_width_)
            max_width_ = modes_[i].width;
        if (modes_[i].height > max_height_)
            max_height_ = modes_[i].height;
        if (modes_[i].redBits > max_red_)
            max_red_ = modes_[i].redBits;
        if (modes_[i].greenBits > max_green_)
            max_green_ = modes_[i].greenBits;
        if (modes_[i].blueBits > max_blue_)
            max_blue_ = modes_[i].blueBits;
        if (modes_[i].refreshRate > max_refresh_)
            max_refresh_ = modes_[i].refreshRate;

#ifdef DEBUG
        std::stringstream ss;
        ss << modes_[i].width << ", " << modes_[i].height << ", " << modes_[i].redBits << ", " << modes_[i].greenBits << ", " << modes_[i].blueBits << ", " << modes_[i].refreshRate;
        DEBUG_NAME_PRINT("[WINDOW] width, height, red, green, blue, refresh", ss.str());
#endif
    }

    width_ = max_width_ / 2;
    height_ = max_height_ / 2;
    red_ = max_red_;
    green_ = max_green_;
    blue_ = max_blue_;
    refresh_ = max_refresh_;
}