#include "Laser.h"
#include "Ship.h"

using namespace Zeni::Collision;

#define LASER_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define LASER_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))

Laser::Laser(const Point3f &m_corner_, const Zeni::Vector3f &m_scale_,
	const Quaternion &rotation_)
	: m_source(new Sound_Source(get_Sounds()["laser_strike"])),
	m_corner(m_corner_),
	m_scale(m_scale_),
	m_rotation(rotation_),
	m_can_destroy(false)
{
	if (!m_instance_count)
		m_model = new Model("models/crate.3ds");
	++m_instance_count;

	expire.start();

	m_velocity = (rotation_ * LASER_DEFAULT_FORWARD_VECTOR) * 100.0f;

	create_body();
}

Laser::~Laser() {
	delete m_source;

	if (!--m_instance_count) {
		delete m_model;
		m_model = nullptr;
	}
}

Vector3f Laser::get_forward() const {
	return m_rotation * LASER_DEFAULT_FORWARD_VECTOR;
}

Vector3f Laser::get_up() const {
	return m_rotation * LASER_DEFAULT_UP_VECTOR;
}

void Laser::step(const float &time_step) {
	if (expire.seconds() > 10.0f || exploding.seconds() > 2.0f)
		m_can_destroy = true;

	if (!exploding.seconds()) {
		m_corner += time_step * m_velocity;
		create_body();
	}
}

void Laser::create_body() {
	m_body = Parallelepiped(m_corner,
		m_rotation * m_scale.get_i(),
		m_rotation * m_scale.get_j(),
		m_rotation * m_scale.get_k());

	m_source->set_position(m_corner + m_rotation * m_scale / 2.0f);
}

void Laser::collide() {
	if (!exploding.seconds())
		m_source->play();
	expire.stop();
	exploding.start();
}

void Laser::render() const {
	if (exploding.seconds())
		return;
	auto rotation = m_rotation.get_rotation();

	m_model->set_translate(m_corner);
	m_model->set_scale(m_scale);
	m_model->set_rotate(rotation.second, rotation.first);

	m_model->render();
}

bool Laser::intersects(const Collision::Parallelepiped &p) const {
	if (exploding.seconds())
		return false;

	return m_body.intersects(p);
}

Model * Laser::m_model = nullptr;
unsigned long Laser::m_instance_count = 0lu;