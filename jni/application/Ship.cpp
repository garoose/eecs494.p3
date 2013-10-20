#include "Ship.h"

using namespace Zeni::Collision;

#define SHIP_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define SHIP_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))

Ship::Ship(const Point3f &m_position_, const Vector3f &m_size_)
	: m_position(m_position_),
	m_size(m_size_),
	m_is_on_ground(false)
{
	create_body();
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

void Ship::adjust_pitch(const float &phi) {
	//FIXME don't know what I'm doing
	m_orientation *= phi;
}

void Ship::adjust_roll(const float &rho) {
	m_orientation *= rho;
}

void Ship::turn_left_xy(const float &theta) {
	//m_camera.turn_left_xy(theta);
}

void Ship::step(const float &time_step) {
	m_position += time_step * m_velocity;
	create_body();
}

void Ship::create_body() {
	Sound &sr = get_Sound();

	Point3f p0(m_position.x + m_size.x, m_position.y, m_position.z);
	Point3f p1(m_position.x, m_position.y + m_size.y, m_position.z);
	Point3f p2(m_position.x, m_position.y, m_position.z + m_size.z);

	m_body = Parallelepiped(m_position, p0, p1, p2);

	sr.set_listener_position(m_position);
	sr.set_listener_forward_and_up(get_forward(), get_up());
	sr.set_listener_velocity(m_velocity);
}

void Ship::render_side(const Point3f &point, const Vector3f &normal) {
	Video &vr = get_Video();

	Vector3f dx = normal.get_i();
	Vector3f dy = normal.get_j();
	Vector3f dz = normal.get_k();

	Color c = get_Colors()["red"];

	Vertex3f_Color p0(point, c);
	Vertex3f_Color p1(point + dx, c);
	Vertex3f_Color p2(point + dy, c);
	Vertex3f_Color p3(point + dz, c);

	Quadrilateral<Vertex3f_Color> q(p0, p1, p2, p3);

	vr.render(q);
}

void Ship::render() {
	Point3f point = get_position();
	Vector3f normal = get_size();

	Vector3f dx = normal.get_i();
	Vector3f dy = normal.get_j();
	Vector3f dz = normal.get_k();

	render_side(point, point + dx);
	render_side(point, point + dy);
	render_side(point, point + dz);
}
