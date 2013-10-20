#include "Player.h"

Player::Player(const Camera &camera_, const Vector3f &size_,
	const Vector3f &camera_offset_, const float &max_speed_, const float &acceleration_)
	: m_camera(camera_),
	m_camera_offset(camera_offset_),
	Ship(m_camera.position - m_camera_offset, size_, max_speed_, acceleration_)
	//m_model(model_file_)
{
	m_camera.fov_rad = Zeni::Global::pi / 3.0f;
}

// Level 2
void Player::set_position(const Point3f &position) {
	m_camera.position = position;

	Ship::set_position(position);
}

void Player::adjust_pitch(const float &phi) {
	m_camera.adjust_pitch(phi);

	Ship::adjust_pitch(phi);
}

void Player::adjust_roll(const float &rho) {
	m_camera.adjust_roll(rho);

	Ship::adjust_roll(rho);
}

void Player::adjust_yaw(const float &theta) {
	m_camera.adjust_yaw(theta);

	Ship::adjust_yaw(theta);
}

void Player::turn_left_xy(const float &theta) {
	m_camera.turn_left_xy(theta);
	
	Ship::turn_left_xy(theta);
}

// Level 3
void Player::step(const float &time_step) {
	m_camera.position += time_step * get_velocity();
	
	Ship::step(time_step);
}

void Player::render() {
	Ship::render();
}