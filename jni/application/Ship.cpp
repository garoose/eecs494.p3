#include "Ship.h"

using namespace Zeni::Collision;

#define SHIP_DEFAULT_FORWARD_VECTOR       (Vector3f(1.0f, 0.0f, 0.0f))
#define SHIP_DEFAULT_UP_VECTOR            (Vector3f(0.0f, 0.0f, 1.0f))

Ship::Ship(const Point3f &m_position_, const Vector3f &m_size_,
		const float &m_max_speed_, const float &m_acceleration_)
	: m_position(m_position_),
	m_size(m_size_),
	m_max_speed(m_max_speed_),
	m_acceleration(m_acceleration_)
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

void Ship::adjust_yaw(const float &theta) {
	m_orientation *= theta;
}

void Ship::turn_left_xy(const float &theta) {
}

void Ship::step(const float &time_step) {
	m_position += time_step * m_velocity;
	create_body();
}

void Ship::create_body() {
	Sound &sr = get_Sound();

	Vector3f size = m_orientation * m_size;

	Point3f p0(m_position.x + size.x, m_position.y, m_position.z);
	Point3f p1(m_position.x, m_position.y + size.y, m_position.z);
	Point3f p2(m_position.x, m_position.y, m_position.z + size.z);

	m_body = Parallelepiped(m_position, p0, p1, p2);

	sr.set_listener_position(m_position);
	sr.set_listener_forward_and_up(get_forward(), get_up());
	sr.set_listener_velocity(m_velocity);
}

void Ship::render_side(const Point3f &point, const Vector3f &a, const Vector3f &b, const Vector3f &c, Color &col) {
	Video &vr = get_Video();

	//Color col = get_Colors()["red"];

	Vertex3f_Color p0(point, col);
	Vertex3f_Color p1(point + a, col);
	Vertex3f_Color p2(point + b, col);
	Vertex3f_Color p3(point + c, col);

	Quadrilateral<Vertex3f_Color> q(p0, p1, p2, p3);

	vr.render(q);
}

void Ship::render() {
	Point3f point = get_position();
	Vector3f normal = m_orientation * get_size();

	Vector3f dx = normal.get_i();
	Vector3f dy = normal.get_j();
	Vector3f dz = normal.get_k();

	render_side(point, point + dx, point + dx + dy, point + dy, get_Colors()["red"]);
	render_side(point, point + dy, point + dy + dz, point + dz, get_Colors()["blue"]);
	render_side(point, point + dz, point + dx + dz, point + dx, get_Colors()["purple"]);

	render_side(point + dx, point + dz, point + dy + dz, point + dy, get_Colors()["yellow"]);
	render_side(point + dy, point + dz, point + dx + dz, point + dx, get_Colors()["magenta"]);
	render_side(point + dz, point + dx, point + dx + dy, point + dy, get_Colors()["orange"]);
}
