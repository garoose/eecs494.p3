#pragma once

#include <zenilib.h>

#include "Ship.h"

using namespace Zeni;

class Player : public Ship {
	Camera m_camera;
	Vector3f m_camera_offset;
	Light m_light;

public:
	Player(const Camera &camera_, const Vector3f &size_,
	const float &max_speed_ = 750.0f, const float &acceleration_ = 0.6f);

	// Level 1
	const Zeni::Camera & get_camera() const { return m_camera; }

	// Level 2
	void set_position(const Zeni::Point3f &position);
	void reset() override;

	void adjust_pitch(const float &phi);
	void adjust_roll(const float &rho);
	void adjust_yaw(const float &theta);
	void turn_left_xy(const float &theta);

	// Level 3
	void step(const float &time_step);
	void render();
};