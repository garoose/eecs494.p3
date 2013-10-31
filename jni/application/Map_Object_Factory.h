#pragma once

#include <zenilib.h>

#include "Wall.h"
#include "Floor1.h"
#include "Crate.h"

using namespace Zeni;

class Map_Object_Factory {
public:
	Map_Object_Factory() {}

	Map_Object *make(const String &type, const Point3f &corner_, const Vector3f &scale_ = Vector3f(1.0f, 1.0f, 1.0f),
		const Quaternion &rotation_ = Quaternion::Axis_Angle(Vector3f(0.0f, 0.0f, 1.0f), 0.0f)) {
		if (type == "Wall") {
			return new Wall(corner_, scale_, rotation_);
		}
		if (type == "Floor1") {
			return new Floor1(corner_, scale_, rotation_);
		}
		if (type == "Crate") {
			return new Crate(corner_, scale_, rotation_);
		}

		return nullptr;
	}
};