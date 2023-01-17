#include "common.hpp"
#include "camera.hpp"

Camera::Camera(const float& ratio, const float& near_plane, const float& far_plane, const float& foV, const glm::vec3& position, const unsigned char& camera_mode, const float& camera_distance, const float& view_speed, const float& move_speed)
	: ratio_(ratio)
	, near_(near)
	, far_(far)
	, foV_(foV)
	, angleWidth_(static_cast<float>(MATH::M_PI_2))
	, angleHeight_(0.0f)
	, view_speed_(view_speed)
	, move_speed_(move_speed)
	, camera_type_(CAM::PERSPECTIVE)
	, camera_mode_(camera_mode)
	, camera_distance_(camera_distance)
	, position_(position)
	, projection_(glm::perspective(foV, ratio, near_plane, far_plane)) {
	Update(0.0f, 0.0f, 0, 0, 1.0f);
}

Camera::~Camera() {}

void Camera::ChangeViewSpeed(const float& view_speed) {
	view_speed_ += view_speed;

	if (view_speed_ > 0.15) view_speed_ = 0.15f;
	if (view_speed_ < 0.0015) view_speed_ = 0.0015f;
}

void Camera::ChangeMoveSpeed(const float& move_speed) {
	move_speed_ += move_speed;

	if (move_speed_ > 2) move_speed_ = 2;
	if (move_speed_ < 0.02) move_speed_ = 0.02f;
}

void Camera::ChangeFoV(const float& foV) {
	if (!(camera_type_ | CAM::ORTHOGRAPHIC))	foV_ += foV;

	if (foV_ > MATH::M_PI - MATH::M_PI_8) foV_ = static_cast<float>(MATH::M_PI - MATH::M_PI_8);
	if (foV_ < MATH::M_PI_8) foV_ = static_cast<float>(MATH::M_PI_8);

	ChangeProjection(camera_type_, 0, 0);
}

void Camera::ChangeProjection(const unsigned char& camera_type, const float& near_plane, const float& far_plane) {
	const float left = -10.0;
	const float right = 10.0;
	const float bottom = -10.0; //adapted to ratio
	const float top = 10.0; //adapted to ratio

	if (near_plane) near_ = near_plane;
	if (far_plane) far_ = far_plane;

	if (camera_type & CAM::ORTHOGRAPHIC) {
		camera_type_ = camera_type;
		projection_ = glm::ortho(left, right, bottom - ((top - bottom) * (1.0f / ratio_ - 1.0f) / 2.0f), top + ((top - bottom) * (1.0f / ratio_ - 1.0f) / 2.0f));
	}

	if (camera_type & CAM::PERSPECTIVE) {
		camera_type_ = camera_type;
		projection_ = glm::perspective(foV_, ratio_, near_, far_);
	}

	if (camera_type & CAM::PERSPECTIVE_INFINITE) {
		camera_type_ = camera_type;
		projection_ = glm::infinitePerspective(foV_, ratio_, near_);
	}
}

void Camera::Update(const float& x_rel, const float& y_rel, const int& front, const int& left, const float& frequency) {
	//angle
	angleWidth_ += view_speed_ * static_cast<float>(G_Window->GetWidth() / 2.0f - x_rel) * frequency;
	angleHeight_ -= view_speed_ * static_cast<float>(G_Window->GetHeight() / 2.0f - y_rel) * frequency;
	if (angleHeight_ < -MATH::M_PI_2 + MATH::M_PI_16)	angleHeight_ = static_cast<float>(-MATH::M_PI_2 + MATH::M_PI_16);
	if (angleHeight_ > MATH::M_PI_2 - MATH::M_PI_16)	angleHeight_ = static_cast<float>(MATH::M_PI_2 - MATH::M_PI_16);

	// std::cout << "height: " << angleHeight << std::endl;
	right_ = glm::vec3(sin(angleWidth_ - MATH::M_PI_2), 0, cos(angleWidth_ - MATH::M_PI_2));
	up_ = glm::vec3(cos(angleHeight_ + MATH::M_PI_2) * sin(angleWidth_), sin(angleHeight_ + MATH::M_PI_2), cos(angleHeight_ + MATH::M_PI_2) * cos(angleWidth_));
	//up_ = glm::vec3(0, 1, 0); //to keep camera on a straight height
	direction_ = glm::cross(right_, up_);

	//position
	if (camera_mode_ & CAM::LOCKEDY) {
		position_ += glm::vec3(direction_.x, 0, direction_.z) * static_cast<float>(move_speed_ * front);
	}
	else if (camera_mode_ & CAM::FREEY) {
		position_ += direction_ * static_cast<float>(move_speed_ * front);
	}
	else {
		std::cerr << "Camera: update not possible with this camera mode" << std::endl;
	}
	position_ += right_ * static_cast<float>(move_speed_ * left);

	if (camera_mode_ & CAM::FPS) {
		view_ = glm::lookAt(position_, position_ + direction_, up_);
	}
	else if (camera_mode_ & CAM::TPS) {
		view_ = glm::lookAt(position_ - glm::normalize(direction_) * camera_distance_, position_ + direction_, up_);
	}
	else {
		std::cerr << "Camera: update not possible with this camera mode" << std::endl;
	}
}

void Camera::SetLooking() {
	GLfloat depth;

	glReadPixels(G_Window->GetWidth() / 2, G_Window->GetHeight() / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

	glm::vec4 window_coords(G_Window->GetWidth() / 2.0f, G_Window->GetHeight() / 2.0f, depth, 1.0f);
	glm::vec4 clip_coords((window_coords.x * 2.0f) / G_Window->GetWidth() - 1.0f, 1.0f - (window_coords.y * 2.0f) / G_Window->GetHeight(), 2.0f * depth - 1.0f, 1.0f);
	glm::vec4 view_coords(glm::inverse(getP()) * clip_coords);
	glm::vec4 looking_point(glm::inverse(getV()) * view_coords);
	looking_point.x /= looking_point.w;
	looking_point.y /= looking_point.w;
	looking_point.z /= looking_point.w;

	looking_point_ = looking_point;

	// DEBUG_NAME_PRINT("looking_point x", looking_point_.x);
	// DEBUG_NAME_PRINT("looking_point y", looking_point_.y);
	// DEBUG_NAME_PRINT("looking_point z", looking_point_.z);

	looking_distance_ = glm::sqrt((position_.x - looking_point_.x) * (position_.x - looking_point_.x)
		+ (position_.y - looking_point_.y) * (position_.y - looking_point_.y)
		+ (position_.z - looking_point_.z) * (position_.z - looking_point_.z));

// DEBUG_NAME_PRINT("looking_distance", looking_distance_);
}