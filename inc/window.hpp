#pragma once

class Window {
    public:
    Window();
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    GLFWwindow* const GetWindow() const { return window_; }
    int const GetWidth() const { return width_; }
    int const GetHeight() const { return height_; }
    int const GetRefresh() const { return refresh_; }
    private:
    void updateInfos();

    GLFWmonitor** monitors_;
    const GLFWvidmode* modes_;
    GLFWwindow* window_;

    int width_;
    int height_;

    int red_;
    int green_;
    int blue_;

    int refresh_;

    int max_width_;
    int max_height_;

    int max_red_;
    int max_green_;
    int max_blue_;

    int max_refresh_;

    int monitor_count_;
    int video_count_;

    bool fullscreen_;
};
