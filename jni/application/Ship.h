#pragma once

#include <zenilib.h>

using namespace Zeni;

class Ship {
	Point3f m_position;
	Vector3f m_size;
	//Model m_model;

	float m_speed;
	Quaternion m_rotation;

public:
	Ship(const Point3f &pos_, const Vector3f &size_, const String &model_file_, const float &speed_ = 0.0f,
		const Quaternion &rotation_ = Quaternion() )
		: m_position(pos_), m_size(size_),// m_model(model_file_),
		m_speed(speed_), m_rotation(rotation_)
	{
	}

	const Point3f &get_position() { return m_position; }
	const Vector3f &get_size() { return m_size; }
	const float &get_speed() { return m_speed; }

	virtual void render();

private:
	void render_side(const Point3f &top_left, const Point3f &bottom_right);
};