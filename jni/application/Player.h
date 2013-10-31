#pragma once

#include <zenilib.h>

#include "Ship.h"

using namespace Zeni;

class Player : public Ship {
	Camera m_camera;
	Vector3f m_camera_offset;
	Vector3f m_camera_offset_max;
	Light m_light;

	bool m_reversed;

	Collision::Parallelepiped m_camera_body; // collision

	void _set_position();

protected:
	void create_body() override;

public:
	Player(const Camera &camera_, const Vector3f &size_,
		const float &max_speed_ = 20000.0f, const float &acceleration_ = 2.0f,
		const Vector3f &camera_offset_ = Vector3f(45.0f, 0.0f, 14.0f));

	// Level 1
	const Zeni::Camera & get_camera() const { return m_camera; }
	const Collision::Parallelepiped &get_camera_body() { return m_camera_body; }

	// Level 2
	void set_position(const Zeni::Point3f &position);
	void adjust_camera_offset(const Vector3f &delta);
	void reset() override;

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void adjust_yaw(const float &theta);

	void reverse_camera();

	// Level 3
	void step(const float &time_step);
	void render() const;
	void render_hp() const;
};