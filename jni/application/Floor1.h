#pragma once

#include <zenilib.h>
#include "Wall_Base.h"

using namespace Zeni;

class Ship;
class Laser;

class Floor1 : public Wall_Base {

public:
	Floor1(const Zeni::Point3f &corner_ = Zeni::Point3f(0.0f, 0.0f, 0.0f),
		const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f, 1.0f, 1.0f),
		const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f, 0.0f, 1.0f), 0.0f))
		: Wall_Base(corner_, scale_, rotation_, "models/floor1.3ds")
	{
	}

	const std::string get_type() const override { return "Wall"; }

private:
};