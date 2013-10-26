#pragma once

#include <zenilib.h>

#include "Laser.h"

using namespace Zeni;

class Ship {
	Point3f m_reset_pos;
	Chronometer<Time> laser_cooldown;

	// Level 1/2
	Point3f m_corner;
	Quaternion m_rotation;

	Sound_Source *m_source;
	int m_health;
	bool m_exploded;

	// Level 2
	Vector3f m_scale;

	// Level 3
	Collision::Parallelepiped m_body; // collision
	float m_max_speed;
	float m_acceleration;
	Vector3f m_velocity;

	// Level 4
	// Controls are external to Ship

protected:
	virtual void create_body();

public:
	Ship(const Point3f &m_corner_, const Zeni::Vector3f &m_scale_,
		const float &m_max_speed_, const float &m_acceleration_);

	~Ship();

	int get_health() const { return m_health; }
	bool is_exploded() const { return m_exploded; }

	// Level 1
	static Model *m_model;
	static unsigned long m_instance_count;

	const Point3f &get_position() const { return m_corner;  }
	const Vector3f &get_size() { return m_scale; }
	const Quaternion &get_orientation() { return m_rotation;  }
	const Point3f get_center() { return m_corner + m_rotation * (m_scale / 2.0f); }

	Vector3f get_forward() const;
	Vector3f get_up() const;

	const float &get_acceleration() const { return m_acceleration; }
	const float &get_max_speed() const { return m_max_speed; }

	// Level 2
	void set_position(const Zeni::Point3f &position);
	virtual void reset();

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void adjust_yaw(const float &theta);

	// Level 3
	const Zeni::Collision::Parallelepiped & get_body() const { return m_body; }

	const Zeni::Vector3f & get_velocity() const { return m_velocity; }
	void set_velocity(const Zeni::Vector3f &velocity_) { m_velocity = velocity_; }
	void add_velocity(const Zeni::Vector3f &velocity_) { m_velocity += velocity_; }

	void step(const float &time_step);
	void step(const float &time_step, const Vector3f &velocity);

	virtual void render() const;
	virtual void collide();
	virtual void explode();

	Laser *fire_laser();

private:
	void play_sound(const String &s);
};