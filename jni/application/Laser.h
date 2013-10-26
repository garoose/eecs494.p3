#pragma once

#include <zenilib.h>
#include "Map_Object.h"

class Ship;

using namespace Zeni;

class Laser : public Map_Object {
	void create_body();

	Chronometer<Time> expire;
	Chronometer<Time> exploding;
	bool m_can_destroy;

	// Level 1/2
	Point3f m_corner;
	Quaternion m_rotation;

	// Level 2
	Vector3f m_scale;

	// Level 3
	Collision::Parallelepiped m_body; // collision
	Vector3f m_velocity;

	// Level 4
	// No controls for a laser

public:
	Laser(const Point3f &m_corner_, const Zeni::Vector3f &m_scale_,
		const Quaternion &rotation_ = Quaternion());

	~Laser();

	virtual const std::string get_type() const override { return "Laser"; }

	bool can_destroy() { return m_can_destroy; }

	// Level 1
	static Model *m_model;
	static unsigned long m_instance_count;

	Zeni::Sound_Source * m_source;

	virtual const Point3f &get_corner() const override { return m_corner; }
	virtual const Vector3f &get_scale() const override { return m_scale; }
	virtual const Quaternion &get_rotation() const override { return m_rotation; }
	Vector3f get_forward() const;
	Vector3f get_up() const;

	// Level 2
	void set_position(const Point3f &position);

	// Level 3
	const Collision::Parallelepiped & get_body() const { return m_body; }

	const Vector3f & get_velocity() const { return m_velocity; }
	void set_velocity(const Zeni::Vector3f &velocity_) { m_velocity = velocity_; }

	void step(const float &time_step);
	void render() const override;
	void collide() override;
	bool intersects(const Collision::Parallelepiped &p) const override;
};