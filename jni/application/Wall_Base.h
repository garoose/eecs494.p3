#pragma once

#include <zenilib.h>
#include <map>

#include "Map_Object.h"

using namespace Zeni;

class Ship;
class Laser;

struct Model_Instance {
	Model *model;
	unsigned long count;

	Model_Instance() {
		model = nullptr;
		count = 0;
	}
};

class Wall_Base : public Map_Object {

public:
	Wall_Base(const Zeni::Point3f &corner_,
		const Zeni::Vector3f &scale_,
		const Zeni::Quaternion &rotation_,
		const String &model_file_);
	Wall_Base(const Wall_Base &rhs);
	Wall_Base & operator=(const Wall_Base &rhs);
	~Wall_Base();

	// Level 1
	const Point3f &get_corner() const override { return m_corner; }
	const Vector3f &get_scale() const override { return m_scale; }
	const Quaternion &get_rotation() const override { return m_rotation; }
	const Point3f get_center() const { return m_corner + (m_rotation * m_size) / 2.0f; }

	void set_translate(const Point3f &position) override { m_corner = position; create_body(); }
	void set_scale(const Vector3f &scale) override { m_scale = scale; create_body(); }
	void set_rotate(const float &angle, const Vector3f &ray) override { m_rotation.space = ray; m_rotation.time = angle; create_body(); };

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void adjust_yaw(const float &theta);

	void step(const float &time_step) override;
	void render() const override;
	bool intersects(const Collision::Parallelepiped &p) const override;
	void collide() override;

	const Zeni::Collision::Parallelepiped & get_body() const { return m_body; }

private:
	void create_body();

	// Level 1
	static std::map<String, Model_Instance> m_model_map;
	Model_Instance *m_instance;
	Model *m_model;

	// Level 2
	Zeni::Point3f m_corner;
	Zeni::Vector3f m_scale;
	Zeni::Vector3f m_size;
	Zeni::Quaternion m_rotation;

	// Level 3
	Zeni::Collision::Parallelepiped m_body; // not motion so much as collision

	// Level 4
	// A stationary Crate has no controls
};