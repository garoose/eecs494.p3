#pragma once

#include <zenilib.h>

#include "Ship.h"

using namespace Zeni;

class Player : public Ship {
	Camera m_camera;
	Vector3f m_camera_offset;

public:
	Player(const Point3f &pos_, const Vector3f &size_, const String &model_file_, 
			const Vector3f &camera_offset_ = Vector3f(0.0f, 0.0f, 0.0f), const float &speed_ = 100.0f )
		: Ship(pos_, size_, model_file_, speed_),
		m_camera_offset(camera_offset_),
		m_camera(get_position() + m_camera_offset)
		//m_model(model_file_)
	{
	}
};