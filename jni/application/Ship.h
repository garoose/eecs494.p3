#pragma once

#include <zenilib.h>

using namespace Zeni;

class Ship {
	void create_body();

	Point3f m_reset_pos;

	// Level 1/2
	Point3f m_position;
	Quaternion m_orientation;

	// Level 2
	Vector3f m_size;
	float m_radius;

	// Level 3
	Collision::Parallelepiped m_body; // collision
	float m_max_speed;
	float m_acceleration;
	Vector3f m_velocity;

	// Level 4
	// Controls are external to Player

public:
	Ship(const Point3f &m_position_, const Zeni::Vector3f &m_size_,
		const float &m_max_speed_, const float &m_acceleration_);

	~Ship();

	// Level 1
	static Model *m_model;
	static unsigned long m_instance_count;

	const Point3f &get_position() const { return m_position;  }
	const Vector3f &get_size() { return m_size; }
	Vector3f Ship::get_forward() const;
	Vector3f Ship::get_up() const;

	const float &get_acceleration() const { return m_acceleration; }
	const float &get_max_speed() const { return m_max_speed; }

	// Level 2
	void set_position(const Zeni::Point3f &position);
	virtual void reset();

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void adjust_yaw(const float &theta);
	void turn_left_xy(const float &theta);

	// Level 3
	const Zeni::Collision::Parallelepiped & get_body() const { return m_body; }

	const Zeni::Vector3f & get_velocity() const { return m_velocity; }
	void set_velocity(const Zeni::Vector3f &velocity_) { m_velocity = velocity_; }
	void add_velocity(const Zeni::Vector3f &velocity_) { m_velocity += velocity_; }

	void step(const float &time_step);

	virtual void render();

private:
	void render_side(const Point3f &point, const Vector3f &a, const Vector3f &, const Vector3f &c, Color &col);
};