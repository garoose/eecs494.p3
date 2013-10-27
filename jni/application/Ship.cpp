#include <zenilib.h>
#include "Ship.h"

using namespace Zeni;
using namespace Zeni::Collision;

#define SHIP_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define SHIP_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))
#define SHIP_DEFAULT_LEFT_VECTOR          (Vector3f(0.0f, 1.0f, 0.0f))

const String collide_sfx = "collide";
const String explode_sfx = "explode";
const String laser_sfx = "retro_laser";

Ship::Ship(const Point3f &m_corner_, const Vector3f &m_scale_,
	const float &m_max_speed_, const float &m_acceleration_)
	: m_source(new Sound_Source(get_Sounds()["collide"])),
	m_corner(m_corner_),
	m_scale(m_scale_),
	m_max_speed(m_max_speed_),
	m_acceleration(m_acceleration_),
	m_reset_pos(m_corner_),
	m_health(100.0f),
	m_exploded(false)
{
	if (!m_instance_count) {
		m_model = new Model("models/crate.3ds");
		m_explosion = new Model("models/explosion.3ds");
	}
	++m_instance_count;

	m_headlight.set_light_type(LIGHT_SPOT);
	//m_headlight.set_spot_phi(Global::pi);
	//m_headlight.set_spot_theta(Global::pi);
	m_headlight.diffuse = Color(1.0f, 0.8f, 0.8f, 0.8f);
	m_headlight.specular = Color(1.0f, 0.8f, 0.8f, 0.8f);
	//m_headlight.linear_attenuation = 0.001f;
	//m_headlight.quadratic_attenuation = 0.000001f;

	create_body();
}

Ship::~Ship() {
	if (!--m_instance_count) {
		delete m_model;
		delete m_explosion;
		m_model = nullptr;
	}
}

Vector3f Ship::get_forward() const {
	return m_rotation * SHIP_DEFAULT_FORWARD_VECTOR;
}

Vector3f Ship::get_up() const {
	return m_rotation * SHIP_DEFAULT_UP_VECTOR;
}

Vector3f Ship::get_left() const {
	return m_rotation * SHIP_DEFAULT_LEFT_VECTOR;
}

// Level 2
void Ship::set_position(const Point3f &position) {
	m_corner = position;
	create_body();
}

void Ship::reset() {
	m_corner = m_reset_pos;
	m_rotation = Quaternion();
	m_velocity = Vector3f();
	m_health = 100.0f;
	m_exploded = false;
	m_exploding.stop();
	m_exploding.set(0.0f);
}

void Ship::adjust_pitch(const float &phi) {
	if (m_exploding.seconds())
		return;
	m_rotation *= Quaternion(0.0f, phi, 0.0f, 0.0f);
}

void Ship::adjust_roll(const float &rho) {
	if (m_exploding.seconds())
		return;
	m_rotation *= Quaternion(0.0f, 0.0f, rho, 0.0f);
}

void Ship::adjust_yaw(const float &theta) {
	if (m_exploding.seconds())
		return;
	m_rotation *= Quaternion(theta, 0.0f, 0.0f, 0.0f);
}

void Ship::step(const float &time_step) {
	if (m_exploding.seconds()) {
		m_explosion->set_keyframe(m_exploding.seconds() * 80.0f);
		if (m_exploding.seconds() > 2.5f)
			m_exploded = true;
		return;
	}

	m_corner += time_step * m_velocity;
	create_body();
}

void Ship::create_body() {
	auto rotation = m_rotation.get_rotation();

	m_model->set_translate(m_corner);
	m_model->set_scale(m_scale);
	m_model->set_rotate(rotation.second, rotation.first);

    m_body = Parallelepiped(m_corner,
                            m_rotation * m_scale.get_i(),
							m_rotation * m_scale.get_j(),
							m_rotation * m_scale.get_k());

	m_source->set_position(m_corner + (m_rotation * (m_scale / 2.0f)));

	m_headlight.position = m_corner + (m_rotation * m_scale) / 2.0f - (get_forward().normalize() * 100.0f);
	m_headlight.spot_direction = get_forward();
}

void Ship::render() const {
	if (m_exploding.seconds() <= 0.1f)
		m_model->render();

	if (m_exploding.seconds()) {
		m_explosion->render();
	}
}

void Ship::collide() {
	play_sound(collide_sfx);

	m_health -= 20.0f;

	if (m_health <= 0.0f) {
		explode();
	}
}

void Ship::explode() {
	m_exploding.start();
	m_explosion->set_translate(get_center());

	play_sound(explode_sfx);
}

Laser *Ship::fire_laser() {
	if (laser_cooldown.seconds() == 0.0f) {
		Vector3f endpt = Vector3f(1.0f, 1.0f, 1.0f);// = (get_forward().normalize().multiply_by(Vector3f(10.0f, 2.0f, 2.0f)));
		Laser *l = new Laser(get_center(), endpt, m_rotation);
		play_sound(laser_sfx);
		laser_cooldown.start();

		return l;
	}
	else {
		if (laser_cooldown.seconds() > 1.0f) {
			laser_cooldown.stop();
			laser_cooldown.set(0.0f);
		}

		return nullptr;
	}
}

void Ship::play_sound(const String &s) {
	if (m_source->is_playing())
		m_source->stop();
	m_source->set_buffer(get_Sounds()[s]);
	m_source->play();
}

Model * Ship::m_model = nullptr;
Model * Ship::m_explosion = nullptr;
unsigned long Ship::m_instance_count = 0lu;