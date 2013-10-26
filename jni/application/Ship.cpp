#include <zenilib.h>
#include "Ship.h"

using namespace Zeni;
using namespace Zeni::Collision;

#define SHIP_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define SHIP_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))

const String collide_sfx = "collide";
const String explode_sfx = "explode";
const String laser_sfx = "retro_laser";

Ship::Ship(const Point3f &m_position_, const Vector3f &m_size_,
	const float &m_max_speed_, const float &m_acceleration_)
	: m_source(new Sound_Source(get_Sounds()["collide"])),
	m_position(m_position_),
	m_size(m_size_),
	m_max_speed(m_max_speed_),
	m_acceleration(m_acceleration_),
	m_reset_pos(m_position_),
	m_health(100.0f),
	m_exploded(false)
{
	if (!m_instance_count)
		m_model = new Model("models/crate.3ds");
	++m_instance_count;

	create_body();
}

Ship::~Ship() {
	if (!--m_instance_count) {
		delete m_model;
		m_model = nullptr;
	}
}

Vector3f Ship::get_forward() const {
	return m_orientation * SHIP_DEFAULT_FORWARD_VECTOR;
}

Vector3f Ship::get_up() const {
	return m_orientation * SHIP_DEFAULT_UP_VECTOR;
}

// Level 2
void Ship::set_position(const Point3f &position) {
	m_position = position;
	create_body();
}

void Ship::reset() {
	m_position = m_reset_pos;
	m_orientation = Quaternion();
	m_velocity = Vector3f();
	m_health = 100.0f;
	m_exploded = false;
}

void Ship::adjust_pitch(const float &phi) {
	m_orientation *= Quaternion(0.0f, phi, 0.0f, 0.0f);
}

void Ship::adjust_roll(const float &rho) {
	m_orientation *= Quaternion(0.0f, 0.0f, rho, 0.0f);
}

void Ship::adjust_yaw(const float &theta) {
	m_orientation *= Quaternion(theta, 0.0f, 0.0f, 0.0f);
}

void Ship::step(const float &time_step) {
	m_position += time_step * m_velocity;
	create_body();
}

void Ship::step(const float &time_step, const Vector3f &velocity) {
	m_position += time_step * (m_velocity + velocity);
	create_body();
}

void Ship::create_body() {
    m_body = Parallelepiped(m_position,
                            m_orientation * m_size.get_i(),
							m_orientation * m_size.get_j(),
							m_orientation * m_size.get_k());

	m_source->set_position(m_position + m_orientation * m_size / 2.0f);
}

void Ship::render() {
	auto rotation = m_orientation.get_rotation();

	m_model->set_translate(m_position);
	m_model->set_scale(m_size);
	m_model->set_rotate(rotation.second, rotation.first);

	m_model->render();
}

void Ship::collide() {
	play_sound(collide_sfx);

	m_health -= 20.0f;

	if (m_health <= 0.0f) {
		explode();
	}
}

void Ship::explode() {
	m_exploded = true;

	play_sound(explode_sfx);
}

Laser *Ship::fire_laser() {
	if (laser_cooldown.seconds() == 0.0f) {
		Vector3f endpt = -get_forward().normalize() + Vector3f(10.0f, 2.0f, 2.0f);
		Laser *l = new Laser(get_center(), endpt, m_orientation);
		play_sound(laser_sfx);
		laser_cooldown.start();

		return l;
	}
	else {
		if (laser_cooldown.seconds() > 1.0f) {
			laser_cooldown.stop();
			laser_cooldown.set(0.0f);
		}
	}
}

void Ship::play_sound(const String &s) {
	if (m_source->is_playing())
		m_source->stop();
	m_source->set_buffer(get_Sounds()[s]);
	m_source->play();
}

Model * Ship::m_model = nullptr;
unsigned long Ship::m_instance_count = 0lu;