#pragma once

#include <zenilib.h>

#include "Map_Object.h"

using namespace Zeni;

class Crate : public Map_Object {
	Point3f m_reset_pos;
	Sound_Source *m_source;

	// Level 1/2
	Point3f m_corner;
	Quaternion m_rotation;

	float m_health;
	bool m_exploded;
	Chronometer<Time> m_exploding;

	// Level 2
	Vector3f m_scale;
	Vector3f m_size;

	// Level 3
	Collision::Parallelepiped m_body; // collision
	Vector3f m_velocity;

	// Level 4
	// A Crate has no controls

protected:
	virtual void create_body();

public:
	Crate(const Zeni::Point3f &m_corner_ = Zeni::Point3f(0.0f, 0.0f, 0.0f),
		const Zeni::Vector3f &m_scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
		const Zeni::Quaternion &m_rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f));

	~Crate();

	const std::string get_type() const override { return "Crate"; }
	const float &get_health() const { return m_health; }
	bool is_exploding() const { return m_exploding.seconds() > 0.0f; }
	bool is_exploded() const { return m_exploded; }

	// Level 1
	static Model *m_model;
	static Model *m_explosion;
	static unsigned long m_instance_count;

	const Point3f &get_corner() const override { return m_corner; }
	const Vector3f &get_scale() const override { return m_scale; }
	const Quaternion &get_rotation() const override { return m_rotation; }
	const Point3f get_center() const { return m_corner + (m_rotation * m_size) / 2.0f; }

	Vector3f get_forward() const;
	Vector3f get_up() const;
	Vector3f get_left() const;

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

	void set_translate(const Point3f &position) override { m_corner = position; }
	void set_scale(const Vector3f &scale) override { m_scale = scale; }
	void set_rotate(const float &angle, const Vector3f &ray) override { m_rotation.Axis_Angle(ray, angle); };

	void step(const float &time_step);

	void render() const override;
	bool intersects(const Collision::Parallelepiped &p) const override;
	void collide() override;
	void collide_with_laser() override;
	void explode();

private:
	void play_sound();
};