#include <zenilib.h>
#include "Ship.h"

using namespace Zeni;
using namespace Zeni::Collision;

#define SHIP_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define SHIP_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))
#define SHIP_DEFAULT_LEFT_VECTOR          (Vector3f(0.0f, 1.0f, 0.0f))

Ship::Ship(const Point3f &m_center_, const Vector3f &m_scale_,
	const float &m_max_speed_, const float &m_acceleration_)
	: m_center(m_center_),
	m_scale(m_scale_),
	m_max_speed(m_max_speed_),
	m_acceleration(m_acceleration_),
	m_reset_pos(m_center_),
	m_health(100.0f),
	m_exploded(false),
	m_moved(false),
	m_god_mode(false)
{
	if (!m_instance_count) {
		m_model = new Model("models/ship_centered.3ds");
		m_explosion = new Model("models/explosion.3ds");
	}
	++m_instance_count;

	m_size = m_model->get_extents().upper_bound - m_model->get_extents().lower_bound;
	m_size = m_size.multiply_by(m_scale);

	//initialize sounds
	sounds["collide"] = new Sound_Source(get_Sounds()["collide"]);
	sounds["laser"] = new Sound_Source(get_Sounds()["retro_laser"]);
	sounds["explode"] = new Sound_Source(get_Sounds()["explode"]);
	sounds["engine"] = new Sound_Source(get_Sounds()["engine"]);

	m_headlight.set_light_type(LIGHT_SPOT);
	m_headlight.set_spot_phi(Global::pi);
	m_headlight.set_spot_theta(Global::pi_over_two);
	m_headlight.diffuse = Color(1.0f, 0.8f, 0.8f, 0.8f);
	m_headlight.specular = Color(1.0f, 0.8f, 0.8f, 0.8f);
	m_headlight.linear_attenuation = 0.001f;
	//m_headlight.quadratic_attenuation = 0.000001f;

	m_taillight.set_light_type(LIGHT_SPOT);
	m_taillight.set_spot_phi(Global::pi);
	m_taillight.set_spot_theta(Global::pi / 4.0f);
	m_taillight.diffuse = Color(1.0f, 0.8f, 0.0f, 0.0f);
	m_taillight.specular = Color(1.0f, 0.8f, 0.0f, 0.0f);
	m_taillight.linear_attenuation = 0.005f;

	create_body();
}

Ship::~Ship() {
	if (!--m_instance_count) {
		delete m_model;
		delete m_explosion;
		m_model = nullptr;
	}

	for (auto it = sounds.begin(); it != sounds.end();) {
		auto s = it->second;
		it = sounds.erase(it);
		delete s;
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
	m_center = position;
	create_body();
}

void Ship::reset() {
	m_center = m_reset_pos;
	m_rotation = Quaternion();
	m_velocity = Vector3f();
	m_health = 100.0f;
	m_exploded = false;
	m_exploding.stop();
	m_exploding.set(0.0f);
	m_prev_velocity = Vector3f();
}

void Ship::adjust_pitch(const float &phi) {
	if (m_exploding.seconds())
		return;

	m_rotation *= Quaternion(0.0f, phi, 0.0f, 0.0f);
	//m_prev_velocity = Quaternion(0.0f, -phi, 0.0f, 0.0f) * m_prev_velocity;
}

void Ship::adjust_roll(const float &rho) {
	if (m_exploding.seconds())
		return;

	m_rotation *= Quaternion(0.0f, 0.0f, rho, 0.0f);
	//m_prev_velocity = Quaternion(0.0f, 0.0f, rho, 0.0f) * m_prev_velocity;
}

void Ship::adjust_yaw(const float &theta) {
	if (m_exploding.seconds())
		return;

	m_rotation *= Quaternion(theta, 0.0f, 0.0f, 0.0f);
	//m_prev_velocity = Quaternion(theta, 0.0f, 0.0f, 0.0f) * m_prev_velocity;
}

void Ship::step(const float &time_step) {
	if (m_exploding.seconds() > 1.6f) {
		m_exploded = true;
		m_exploding.stop();
		m_exploding.set(0.0f);
	}
	
	if (is_exploded() || is_exploding())
		return;

	m_center += time_step * m_velocity;

	if (m_moved)
		play_sound("engine", false);
	else
		stop_sound("engine");

	create_body();
}

void Ship::create_body() {
	// Create the collision object
	m_body = Parallelepiped(get_corner(),
		m_rotation * m_size.get_i(),
		m_rotation * m_size.get_j(),
		m_rotation * m_size.get_k());

	// Set all sound sources to center of the ship
	for (auto it = sounds.begin(); it != sounds.end(); ++it)
		it->second->set_position(get_center());

	// Adjust the headlight to be behind and facing forward
	m_headlight.position = get_center() + (get_forward().normalize() * (-m_size.x));
	m_headlight.spot_direction = get_forward();

	// Adjust the taillight to be behing and facing back
	m_taillight.position = get_center() + (get_forward().normalize() * (-m_size.x));
	m_taillight.spot_direction = -get_forward();
}

void Ship::render() const {
	if (m_exploded)
		return;

	// Position and render the ship model
	auto rotation = m_rotation.get_rotation();

	m_model->set_translate(m_center);
	m_model->set_scale(m_scale);
	m_model->set_rotate(rotation.second, rotation.first);

	if (m_exploding.seconds() <= 0.1f)
		m_model->render();

	if (is_exploding()) {
		m_explosion->set_keyframe(m_exploding.seconds() * 80.0f);
		m_explosion->set_translate(get_center());
		m_explosion->render();
	}
}

void Ship::collide() {
	if (m_health <= 0.0f)
		return;

	play_sound("collide");

	float damage = (100.0f * (m_velocity.magnitude() / (m_max_speed / 10.0f)) + 1.0f);

	m_health -= m_god_mode ? 0.0f : damage;
	//m_health -= m_god_mode ? 0.0f : 10.0f;

	if (m_health <= 0.0f) {
		explode();
	}
}

void Ship::collide_with_laser() {
	m_health -= m_god_mode ? 0.0f : 10.0f;

	if (m_health <= 0.0f) {
		explode();
	}
}

void Ship::explode() {
	m_exploding.start();
	m_explosion->set_translate(get_center());

	stop_all_sounds();
	play_sound("explode", false);
	create_body();
}

bool Ship::intersects(const Parallelepiped &s) const {
	if (!can_collide())
		return false;

	return m_body.intersects(s);
}

Laser *Ship::fire_laser() {
	if (is_exploded() || is_exploding())
		return nullptr;

	if (laser_cooldown.seconds() == 0.0f) {
		Vector3f endpt = Vector3f(1.0f, 1.0f, 1.0f);
		Point3f pos = get_center() + get_forward() * (get_size().x / 2.0f) + get_up() * get_size().z;
		Vector3f vel = get_forward() * 400.0f;
		Laser *l = new Laser(pos, endpt, m_rotation, vel);
		play_sound("laser");
		laser_cooldown.start();

		return l;
	}
	else {
		if (laser_cooldown.seconds() > 0.5f) {
			laser_cooldown.stop();
			laser_cooldown.set(0.0f);
		}

		return nullptr;
	}
}

void Ship::play_sound(const String &s, bool stop_current) {
	Sound_Source *m_source = sounds.find(s)->second;

	if (m_source->is_playing()) {
		if (!stop_current)
			return;
		m_source->stop();
	}

	m_source->play();
}

void Ship::stop_sound(const String &s) {
	Sound_Source *m_source = sounds.find(s)->second;

	m_source->stop();
}

void Ship::stop_all_sounds() {
	for (auto it = sounds.begin(); it != sounds.end(); ++it)
		it->second->stop();
}

Model * Ship::m_model = nullptr;
Model * Ship::m_explosion = nullptr;
unsigned long Ship::m_instance_count = 0lu;