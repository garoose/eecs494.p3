#pragma once

#include <zenilib.h>
#include <map>

#include "Laser.h"

using namespace Zeni;

class Ship {
	Point3f m_reset_pos;
	Chronometer<Time> laser_cooldown;
	bool m_moved;

	// Level 1/2
	Point3f m_center;
	Quaternion m_rotation;

	std::map<String, Sound_Source *> sounds;
	float m_health;
	bool m_god_mode;
	bool m_exploded;
	Chronometer<Time> m_exploding;

	// Level 2
	Vector3f m_scale;
	Vector3f m_size;

	// Level 3
	Collision::Parallelepiped m_body; // collision
	Light m_headlight;
	Light m_taillight;
	float m_max_speed;
	float m_acceleration;
	Vector3f m_velocity;
	Vector3f m_prev_velocity;

	// Level 4
	// Controls are external to Ship

protected:
	virtual void create_body();

public:
	Ship(const Point3f &m_corner_, const Zeni::Vector3f &m_scale_,
		const float &m_max_speed_, const float &m_acceleration_);

	~Ship();

	const float &get_health() const { return m_health; }
	void god_mode(const bool &active) { m_god_mode = active; }
	bool is_exploding() const { return m_exploding.seconds() > 0.0f; }
	bool is_exploded() const { return m_exploded; }

	// Level 1
	static Model *m_model;
	static Model *m_explosion;
	static unsigned long m_instance_count;

	const Point3f &get_position() const { return m_center;  }
	const Vector3f &get_size() const { return m_size; }
	const Quaternion &get_orientation() const { return m_rotation;  }
	const Point3f get_center() const { return m_center; }
	const Point3f get_corner() const { return m_center - (m_rotation * m_size) / 2.0f; }

	Vector3f get_forward() const;
	Vector3f get_up() const;
	Vector3f get_left() const;

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
	const Light &get_headlight() const { return m_headlight; }
	const Light &get_taillight() const { return m_taillight; }

	const Vector3f & get_velocity() const { return m_velocity; }
	const Vector3f &get_prev_velocity() const { return m_prev_velocity; }
	void set_velocity(const Vector3f &velocity_) { m_velocity = velocity_; }
	void add_velocity(const Vector3f &velocity_) { m_velocity += velocity_; }
	void add_prev_velocity(const Vector3f &velocity_) { m_prev_velocity += velocity_; }
	void clear_prev_velocity() { m_prev_velocity = Vector3f(); }

	void step(const float &time_step);

	virtual void render() const;
	virtual void collide();
	virtual void collide_with_laser();
	virtual void explode();

	bool intersects(const Collision::Parallelepiped &s) const;

	Laser *fire_laser();

	void stop_all_sounds();

	bool moved() const { return m_moved; }
	void set_moved(bool m) { m_moved = m; }

private:
	void play_sound(const String &s, bool stop_current = true);
	void stop_sound(const String &s);
	bool can_collide() const { return (!is_exploding() && !is_exploded()); }
};