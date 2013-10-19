#include "Ship.h"

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

	Point3f p0(top_left.x, top_left.y, bottom_right.z);
	Point3f p1(top_left.x, bottom_right.y, top_left.z);
	Point3f p2(bottom_right.x, top_left.y, top_left.z);

	render_side(top_left, p0);
	render_side(top_left, p1);
	render_side(top_left, p2);
}