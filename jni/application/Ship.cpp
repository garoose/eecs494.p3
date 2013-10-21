#include <zenilib.h>
#include "Ship.h"

using namespace Zeni;
using namespace Zeni::Collision;

#define SHIP_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define SHIP_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))

Ship::Ship(const Point3f &m_position_, const Vector3f &m_size_,
		const float &m_max_speed_, const float &m_acceleration_)
	: m_position(m_position_),
	m_size(m_size_),
	m_max_speed(m_max_speed_),
	m_acceleration(m_acceleration_),
	m_reset_pos(m_position)
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
}

void Ship::adjust_pitch(const float &phi) {
	//FIXME don't know what I'm doing
	m_orientation *= phi;
}

void Ship::adjust_roll(const float &rho) {
	//m_orientation *= rho;
}

void Ship::adjust_yaw(const float &theta) {
	//m_orientation *= theta;
}

void Ship::turn_left_xy(const float &theta) {
}

void Ship::step(const float &time_step) {
	m_position += time_step * m_velocity;
	create_body();
}

void Ship::create_body() {
	Sound &sr = get_Sound();

    m_body = Parallelepiped(m_position,
                            m_orientation * m_size.get_i(),
							m_orientation * m_size.get_j(),
							m_orientation * m_size.get_k());

	sr.set_listener_position(m_position);
	sr.set_listener_forward_and_up(get_forward(), get_up());
	sr.set_listener_velocity(m_velocity);
}

void Ship::render_side(const Point3f &point, const Vector3f &a, const Vector3f &b, const Vector3f &c, Color &col) {
	Video &vr = get_Video();

	Vertex3f_Color p0(point, col);
	Vertex3f_Color p1(point + a, col);
	Vertex3f_Color p2(point + b, col);
	Vertex3f_Color p3(point + c, col);

	Quadrilateral<Vertex3f_Color> q(p0, p1, p2, p3);

	vr.render(q);
}

void Ship::render() {
	Point3f point = get_position();
	Vector3f size = m_orientation * get_size();

	Vector3f dx = size.get_i();
	Vector3f dy = size.get_j();
	Vector3f dz = size.get_k();

	/*render_side(point, dx, dx + dy, dy, get_Colors()["red"]);
	render_side(point, dy, dy + dz, dz, get_Colors()["blue"]);
	render_side(point, dz, dx + dz, dx, get_Colors()["purple"]);

	render_side(point + dx, dz, dy + dz, dy, get_Colors()["yellow"]);
	render_side(point + dy, dz, dx + dz, dx, get_Colors()["magenta"]);
	render_side(point + dz, dx, dx + dy, dy, get_Colors()["orange"]);*/

	auto rotation = m_orientation.get_rotation();

	m_model->set_translate(m_position);
	m_model->set_scale(m_size);
	m_model->set_rotate(rotation.second, rotation.first);

	m_model->render();
}

Model * Ship::m_model = nullptr;
unsigned long Ship::m_instance_count = 0lu;