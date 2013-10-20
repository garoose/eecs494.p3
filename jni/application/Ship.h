#pragma once

#include <zenilib.h>

using namespace Zeni;

class Ship {
	void create_body();

	// Level 1/2
	Point3f m_position;
	Quaternion m_orientation;

	// Level 2
	Vector3f m_size;
	float m_radius;
	//Model m_model;

	// Level 3
	Collision::Parallelepiped m_body; // collision
	Vector3f m_velocity;
	bool m_is_on_ground;

	// Level 4
	// Controls are external to Player

public:
	Ship(const Point3f &m_position_, const Zeni::Vector3f &m_size_);

	// Level 1
	const Point3f &get_position() const { return m_position;  }
	const Vector3f &get_size() { return m_size; }
	Vector3f Ship::get_forward() const;
	Vector3f Ship::get_up() const;

	// Level 2
	void set_position(const Zeni::Point3f &position);

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void turn_left_xy(const float &theta);

	// Level 3
	const Zeni::Collision::Parallelepiped & get_body() const { return m_body; }
	bool is_on_ground() const { return m_is_on_ground; }

	const Zeni::Vector3f & get_velocity() const { return m_velocity; }
	void set_velocity(const Zeni::Vector3f &velocity_) { m_velocity = velocity_; }

	void step(const float &time_step);

	virtual void render();

private:
	void render_side(const Point3f &top_left, const Point3f &bottom_right);
};