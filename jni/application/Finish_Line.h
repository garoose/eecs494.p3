#pragma once

#include <zenilib.h>

#include "Map_Object.h"

using namespace Zeni;

class Finish_Line : public Map_Object {
	bool m_crossed;

public:
	Finish_Line(const Zeni::Point3f &corner_ = Zeni::Point3f(0.0f, 0.0f, 0.0f),
		const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
		const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f));
	~Finish_Line();

	virtual const std::string get_type() const override { return "Finish_Line"; }

	virtual const Point3f &get_corner() const override { return m_corner; }
	virtual const Vector3f &get_scale() const override { return m_scale; }
	virtual const Quaternion &get_rotation() const override { return m_rotation; }
	const Point3f get_center() const { return m_corner + (m_rotation * m_scale) / 2.0f; }

	void set_translate(const Point3f &position) override { m_corner = position; create_body(); }
	void set_scale(const Vector3f &scale) override { m_scale = scale; create_body(); create_body(); }
	void set_rotate(const float &angle, const Vector3f &ray) override { m_rotation.Axis_Angle(ray, angle); create_body(); };

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void adjust_yaw(const float &theta);

	void reset();

	void render() const override;
	bool intersects(const Collision::Parallelepiped &p) const override;
	void collide() override;

	const Zeni::Collision::Parallelepiped & get_body() const { return m_body; }

	bool crossed() const { return m_crossed; }

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