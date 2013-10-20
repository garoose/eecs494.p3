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

void Ship::render_side(const Point3f &top_left, const Point3f &bottom_right) {
	Video &vr = get_Video();

	Color c = get_Colors()["red"];

	Vertex3f_Color p0(top_left, c);
	Vertex3f_Color p1(Point3f(bottom_right.x, top_left.y, top_left.z), c);
	Vertex3f_Color p2(Point3f(top_left.x, bottom_right.y, bottom_right.z), c);
	Vertex3f_Color p3(bottom_right, c);

	Quadrilateral<Vertex3f_Color> q(p0, p1, p2, p3);

	vr.render(q);
}

void Ship::render() {
	Point3f top_left = get_position();
	Point3f bottom_right = get_position() + get_size();

	Point3f p0(bottom_right.x, top_left.y, top_left.z);
	Point3f p2(top_left.x, top_left.y, bottom_right.z);
	Point3f p1(top_left.x, bottom_right.y, top_left.z);

	render_side(top_left, p0);
	render_side(top_left, p1);
	render_side(top_left, p2);
}
