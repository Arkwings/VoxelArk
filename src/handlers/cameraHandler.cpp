#include "common.hpp"
#include "camera.hpp"
#include "handlers/cameraHandler.hpp"

CameraHandler::CameraHandler()
    : active_(0) {
    cameras_.push_back(new Camera(16.0f / 9.0f, 1.0f, 200.0f, static_cast<float>(MATH::M_PI_2), glm::vec3(0.0f, OPT::MAP_HEIGHT / 2.0f, 0.0f), CAM::FREEY | CAM::FPS));
    cameras_.push_back(new Camera(16.0f / 9.0f, 1.0f, 200.0f, static_cast<float>(MATH::M_PI_2), glm::vec3(0.0f, OPT::MAP_HEIGHT / 2.0f, 0.0f), CAM::LOCKEDY | CAM::FPS, 5.0f, 0.25f, 0.125f));
    cameras_.push_back(new Camera(16.0f / 9.0f, 1.0f, 200.0f, static_cast<float>(MATH::M_PI_2), glm::vec3(0.0f, OPT::MAP_HEIGHT / 2.0f, 0.0f), CAM::FREEY | CAM::TPS));
    cameras_.push_back(new Camera(16.0f / 9.0f, 1.0f, 200.0f, static_cast<float>(MATH::M_PI_2), glm::vec3(0.0f, OPT::MAP_HEIGHT / 2.0f, 0.0f), CAM::LOCKEDY | CAM::TPS, 5.0f, 0.25f, 0.125f));
}

CameraHandler::~CameraHandler() {
    for (std::vector<Camera*>::iterator it = cameras_.begin(); it != cameras_.end(); ++it) {
        delete (*it);
    }
}
