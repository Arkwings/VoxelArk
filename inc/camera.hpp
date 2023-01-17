#pragma once

class Camera {
	public:
	Camera() = delete;
	Camera(const float& ratio, const float& near_plane, const float& far_plane, const float& _foV, const glm::vec3& position, const unsigned char& camera_mode = CAM::FREEY | CAM::FPS, const float& camera_distance = 5.0f, const float& view_speed = 0.25f, const float& move_speed = 0.75f);
	~Camera();

	void ChangeViewSpeed(const float& view_speed);
	void ChangeMoveSpeed(const float& move_speed);
	void ChangeFoV(const float& foV);
	void ChangeProjection(const unsigned char& camera_type, const float& near_plane, const float& far_plane);
	void Update(const float& x_rel, const float& y_rel, const int& front, const int& left, const float& frequency);
	void SetLooking();

	float& getNear() { return near_; }
	float& getFar() { return far_; }
	glm::mat4& getP() { return projection_; }
	glm::mat4& getV() { return view_; }
	glm::vec3& getPos() { return position_; }
	glm::vec3& getDir() { return direction_; }
	unsigned char getCam() { return camera_type_; }
	unsigned char getMode() { return camera_mode_; }
	glm::vec3& getLookingPoint() { return looking_point_; }
	GLfloat& getLookingDistance() { return looking_distance_; }
	GLfloat& getCameraDistance() { return camera_distance_; }

	private:
	float ratio_;
	float near_;
	float far_;
	float foV_;

	float angleWidth_;
	float angleHeight_;

	float view_speed_;
	float move_speed_;

	unsigned char camera_type_;
	unsigned char camera_mode_;
	float camera_distance_;	//only used for tps cam

	glm::vec3 right_;
	glm::vec3 up_;
	glm::vec3 position_;
	glm::vec3 direction_;

	glm::mat4 projection_;
	glm::mat4 view_;

	glm::vec3 looking_point_;
	GLfloat looking_distance_;
};
