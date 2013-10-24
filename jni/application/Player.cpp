#include "Player.h"

Player::Player(const Camera &camera_, const Vector3f &size_, const float &max_speed_, const float &acceleration_)
	: m_camera(camera_),
	m_camera_offset(3.0f),
	Ship(m_camera.position, size_, max_speed_, acceleration_)
{
	m_camera.fov_rad = Zeni::Global::pi / 3.0f;
}

void Player::_set_position() {
	Vector3f offset = -get_forward().normalize() + get_up().normalize();
	m_camera.position = get_position() + (get_center()) + (offset * m_camera_offset);
}

// Level 2
void Player::set_position(const Point3f &position) {
	Ship::set_position(position);

	_set_position();
}

void Player::reset() {
	Ship::reset();

	set_position(get_position());
	m_camera.orientation = Quaternion();

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

	_set_position();
}

void Player::render() {
	Ship::render();
}