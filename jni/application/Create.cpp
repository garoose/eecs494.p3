#include <zenilib.h>
#include "Crate.h"

using namespace Zeni;
using namespace Zeni::Collision;

#define Crate_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define Crate_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))
#define Crate_DEFAULT_LEFT_VECTOR          (Vector3f(0.0f, 1.0f, 0.0f))

Crate::Crate(const Point3f &m_corner_, const Vector3f &m_scale_,
	const Quaternion &m_rotation_)
	: m_corner(m_corner_),
	m_scale(m_scale_),
	m_rotation(m_rotation_),
	m_reset_pos(m_corner_),
	m_health(100.0f),
	m_exploded(false)
{
	if (!m_instance_count) {
		m_model = new Model("models/crate.3ds");
		m_explosion = new Model("models/explosion.3ds");
	}
	++m_instance_count;

	m_size = m_model->get_extents().upper_bound - m_model->get_extents().lower_bound;
	m_size = m_size.multiply_by(m_scale);

	//initialize sounds
	m_source = new Sound_Source(get_Sounds()["explode"]);

	create_body();
}

Crate::~Crate() {
	if (!--m_instance_count) {
		delete m_model;
		delete m_explosion;
		m_model = nullptr;
	}

	delete m_source;
}

Vector3f Crate::get_forward() const {
	return m_rotation * Crate_DEFAULT_FORWARD_VECTOR;
}

Vector3f Crate::get_up() const {
	return m_rotation * Crate_DEFAULT_UP_VECTOR;
}

Vector3f Crate::get_left() const {
	return m_rotation * Crate_DEFAULT_LEFT_VECTOR;
}

// Level 2
void Crate::set_position(const Point3f &position) {
	m_corner = position;
	create_body();
}

void Crate::reset() {
	m_corner = m_reset_pos;
	m_rotation = Quaternion();
	m_velocity = Vector3f();
	m_health = 100.0f;
	m_exploded = false;
	m_exploding.stop();
	m_exploding.set(0.0f);
}

void Crate::adjust_pitch(const float &phi) {
	if (m_exploding.seconds())
		return;
	m_rotation *= Quaternion(0.0f, phi, 0.0f, 0.0f);
}

void Crate::adjust_roll(const float &rho) {
	if (m_exploding.seconds())
		return;
	m_rotation *= Quaternion(0.0f, 0.0f, rho, 0.0f);
}

void Crate::adjust_yaw(const float &theta) {
	if (m_exploding.seconds())
		return;
	m_rotation *= Quaternion(theta, 0.0f, 0.0f, 0.0f);
}

void Crate::step(const float &time_step) {
	if (m_exploded) {
		create_body();
		return;
	}

	if (m_exploding.seconds()) {
		m_explosion->set_keyframe(m_exploding.seconds() * 80.0f);

		if (m_exploding.seconds() > 2.0f) {
			m_exploded = true;
			m_exploding.stop();
			m_exploding.set(0.0f);
		}
		create_body();
		return;
	}

	m_corner += time_step * m_velocity;
	create_body();
}

void Crate::create_body() {
	// Create the collision object
		m_body = Parallelepiped(m_corner,
			m_rotation * m_size.get_i(),
			m_rotation * m_size.get_j(),
			m_rotation * m_size.get_k());

	// Set sound source to center of the Crate
	m_source->set_position(get_center());
}

void Crate::render() const {
	if (m_exploded)
		return;

	// Position and render the Crate model
	auto rotation = m_rotation.get_rotation();

	m_model->set_translate(m_corner);
	m_model->set_scale(m_scale);
	m_model->set_rotate(rotation.second, rotation.first);

	if (m_exploding.seconds() <= 0.1f)
		m_model->render();

	if (m_exploding.seconds()) {
		m_explosion->render();
	}
}

bool Crate::intersects(const Collision::Parallelepiped &p) const {
	if (is_exploding() || is_exploded())
		return false;

	return m_body.intersects(p);
}

void Crate::collide() {
}

void Crate::collide_with_laser() {
	m_health -= 50.0f;

	if (m_health <= 0.0f) {
		explode();
	}
}

void Crate::explode() {
	m_exploding.start();
	m_explosion->set_translate(get_center());

	play_sound();
}

void Crate::play_sound() {
	if (m_source->is_playing())
		m_source->stop();

	m_source->play();
}

Model * Crate::m_model = nullptr;
Model * Crate::m_explosion = nullptr;
unsigned long Crate::m_instance_count = 0lu;