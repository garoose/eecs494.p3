#include "Laser.h"
#include "Ship.h"

using namespace Zeni::Collision;

#define LASER_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define LASER_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))

Laser::Laser(const Point3f &m_position_, const Zeni::Vector3f &m_size_,
	const Quaternion &rotation_)
	: m_source(new Sound_Source(get_Sounds()["laser_strike"])),
	m_position(m_position_),
	m_size(m_size_),
	m_orientation(rotation_),
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
	return m_orientation * LASER_DEFAULT_FORWARD_VECTOR;
}

Vector3f Laser::get_up() const {
	return m_orientation * LASER_DEFAULT_UP_VECTOR;
}

void Laser::step(const float &time_step) {
	if (expire.seconds() > 10.0f || exploding.seconds() > 2.0f)
		m_can_destroy = true;

	if (!exploding.seconds()) {
		m_position += time_step * m_velocity;
		create_body();
	}
}

void Laser::create_body() {
	Sound &sr = get_Sound();

	m_body = Parallelepiped(m_position,
		m_orientation * m_size.get_i(),
		m_orientation * m_size.get_j(),
		m_orientation * m_size.get_k());

	sr.set_listener_position(m_position);
	sr.set_listener_forward_and_up(get_forward(), get_up());
	sr.set_listener_velocity(m_velocity);

	m_source->set_position(m_position + m_orientation * m_size / 2.0f);
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
	auto rotation = m_orientation.get_rotation();

	m_model->set_translate(m_position);
	m_model->set_scale(m_size);
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