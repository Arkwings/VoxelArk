#pragma once

class Camera;

class CameraHandler {
    public:
    CameraHandler();
    ~CameraHandler();
    CameraHandler(const CameraHandler&) = delete;
    CameraHandler& operator=(const CameraHandler&) = delete;

    Camera* operator [](const unsigned int& ID) { return cameras_[ID]; }
    Camera* GetCam(const unsigned int& ID) { return cameras_[ID]; }
    void SetActive(const unsigned int& ID) { active_ = ID; };
    Camera* GetActive() { return cameras_[active_]; };
    private:
    std::vector<Camera*> cameras_;
    unsigned int active_;
};
