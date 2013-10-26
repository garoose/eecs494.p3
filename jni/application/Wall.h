#pragma once

#include <zenilib.h>
#include "Map_Object.h"

using namespace Zeni;

class Ship;
class Laser;

class Wall : public Map_Object {

public:
	Wall(const Zeni::Point3f &corner_ = Zeni::Point3f(0.0f, 0.0f, 0.0f),
		const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
		const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f));
	Wall(const Wall &rhs);
	Wall & operator=(const Wall &rhs);
	~Wall();

	// Level 1
	const std::string get_type() const override { return "Wall"; }
	const Point3f &get_corner() const override { return m_corner; }
	const Vector3f &get_scale() const override { return m_scale; }
	const Quaternion &get_rotation() const override { return m_rotation; }

	void render() const override;
	bool intersects(const Collision::Parallelepiped &p) const override;
	bool intersects(const Collision::Capsule &c) const override;
	void collide() override;

	const Zeni::Collision::Parallelepiped & get_body() const { return m_body; }

private:
	void create_body();

	// Level 1
	static Zeni::Model * m_model;
	static unsigned long m_instance_count;

	Zeni::Sound_Source * m_source;

	// Level 2
	Zeni::Point3f m_corner;
	Zeni::Vector3f m_scale;
	Zeni::Quaternion m_rotation;

	// Level 3
	Zeni::Collision::Parallelepiped m_body; // not motion so much as collision

	// Level 4
	// A stationary Crate has no controls
};