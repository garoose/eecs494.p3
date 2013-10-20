#pragma once

#include <zenilib.h>
#include "Map_Object.h"

using namespace Zeni;

class Wall : public Map_Object {
	Point3f m_position;
	Vector3f m_length;
	Vector3f m_width;
	Vector3f m_height;

	Color m_color;

	Zeni::Collision::Parallelepiped m_body;

public:
	Wall(const Point3f &pos_, const Vector3f &length_, const Vector3f &width_, const Vector3f &height_, const Color &color_)
		: m_position(pos_),
		m_length(length_),
		m_width(width_),
		m_height(height_),
		m_color(color_)
	{
		create_body();
	}

	void render() const override;
	bool is_intersecting(const Ship &s) const override;

private:
	void create_body();
};