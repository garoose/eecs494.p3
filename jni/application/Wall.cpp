#include "Wall.h"
#include "Ship.h"

void Wall::render() const {
	Video &vr = get_Video();

	Vertex3f_Color p0(m_position, m_color);
	Vertex3f_Color p1(m_length, m_color);
	Vertex3f_Color p2(m_width, m_color);
	Vertex3f_Color p3(m_height, m_color);

	Quadrilateral<Vertex3f_Color> q(p0, p1, p2, p3);

	vr.render(q);
}

bool Wall::is_intersecting(const Ship &s) const {
	return m_body.intersects(s.get_body());
}

void Wall::create_body() {
	m_body = Zeni::Collision::Parallelepiped(m_position, m_length, m_width, m_height);
}