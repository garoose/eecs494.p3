#include "Player.h"

using namespace Zeni::Collision;

Player::Player(const Camera &camera_, const Vector3f &size_, const float &max_speed_, const float &acceleration_, const Vector3f &camera_offset_)
	: m_camera(camera_),
	m_camera_offset(camera_offset_),
	m_camera_offset_max(camera_offset_),
	Ship(camera_.position, size_, max_speed_, acceleration_),
	m_reversed(false)
{
	m_camera.fov_rad = Zeni::Global::pi / 3.0f;
}

void Player::_set_position() {
	Vector3f offset = -get_forward().normalize() * m_camera_offset.x + get_left().normalize() * m_camera_offset.y + get_up().normalize() * m_camera_offset.z;
	m_camera.position = (get_center()) + (offset);
}

// Level 2
void Player::set_position(const Point3f &position) {
	Ship::set_position(position);

	_set_position();
}

void Player::adjust_camera_offset(const Vector3f &delta) {
	m_camera_offset += delta;
	if (m_camera_offset.magnitude() > m_camera_offset_max.magnitude())
		m_camera_offset = m_camera_offset_max;
}

void Player::reset() {
	Ship::reset();

	set_position(get_position());
	m_camera.orientation = Quaternion();
}

void Player::adjust_pitch(const float &phi) {
	if (m_reversed)
		return;

	Ship::adjust_pitch(phi);

	m_camera.adjust_pitch(phi);
	_set_position();
}

void Player::adjust_roll(const float &rho) {
	if (m_reversed)
		return;

	Ship::adjust_roll(rho);

	m_camera.adjust_roll(rho);
	_set_position();
}

void Player::adjust_yaw(const float &theta) {
	if (m_reversed)
		return;

	Ship::adjust_yaw(theta);

	m_camera.adjust_yaw(theta);
	_set_position();
}

void Player::reverse_camera() {
	m_camera.adjust_yaw(Global::pi);
	m_reversed = !m_reversed;
}

// Level 3
void Player::step(const float &time_step) {
	Ship::step(time_step);

	if (is_exploding())
		get_Controllers().set_vibration(0, 0.5f, 0.5f);

	if (is_exploded())
		get_Controllers().set_vibration(0, 0.0f, 0.0f);

	_set_position();
}

void Player::create_body() {
	Ship::create_body();

	// Create the camera collision object
	m_camera_body = Parallelepiped(m_camera.position,
		m_camera.get_forward(),
		m_camera.get_left(),
		m_camera.get_up());

	Sound &sr = get_Sound();

	sr.set_listener_position(m_camera.position);
	sr.set_listener_forward_and_up(m_camera.get_forward(), m_camera.get_up());
	sr.set_listener_velocity(get_velocity());
}

void Player::render() const {
	Ship::render();
}

void Player::render_hp() const {
	Video &vr = get_Video();

	float x = 100.0f;
	float y = get_Window().get_size().y - 100.0f;
	float bar_height = 30.0f;
	float max_bar_size = 300.0f;
	float bar_width = (get_health() / 100.0f) * max_bar_size;

	Vector2f icon_size(64.0f, 64.0f);
	Point2f icon_pos(x - 80.0f, y - 18.0f);
	render_image("health", icon_pos, icon_pos + icon_size);

	Color c = get_Colors()["red"];

	Vertex2f_Color p0(Point2f(x, y), c);
	Vertex2f_Color p1(Point2f(x + bar_width, y), c);
	Vertex2f_Color p2(Point2f(x + bar_width, y + bar_height), c);
	Vertex2f_Color p3(Point2f(x, y + bar_height), c);

	Quadrilateral<Vertex2f_Color> q(p0, p1, p2, p3);

	c = get_Colors()["black"];

	float bg_offset = 2.0f;
	x -= bg_offset;
	y -= bg_offset;
	bar_height += 2 * bg_offset;
	bar_width = max_bar_size + 2 * bg_offset;

	Vertex2f_Color p00(Point2f(x, y), c);
	Vertex2f_Color p11(Point2f(x + bar_width, y), c);
	Vertex2f_Color p22(Point2f(x + bar_width, y + bar_height), c);
	Vertex2f_Color p33(Point2f(x, y + bar_height), c);

	Quadrilateral<Vertex2f_Color> q2(p00, p11, p22, p33);

	vr.render(q2);
	vr.render(q);
}