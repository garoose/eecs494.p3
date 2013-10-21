#include "Player.h"

Player::Player(const Camera &camera_, const Vector3f &size_, const float &max_speed_, const float &acceleration_)
	: m_camera(camera_),
	Ship(m_camera.position - m_camera_offset, size_, max_speed_, acceleration_)
	//m_model(model_file_)
{
	m_camera_offset = Vector3f(get_size().x / 2, get_size().y / 2, get_size().z);

	m_camera.fov_rad = Zeni::Global::pi / 3.0f;
}

// Level 2
void Player::set_position(const Point3f &position) {
	m_camera.position = position - m_camera_offset;

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
	Ship::step(time_step);

	m_camera.position = get_position() + m_camera_offset;
}

void Player::render() {
	Ship::render();
}