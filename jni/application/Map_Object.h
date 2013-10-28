#pragma once

#include <zenilib.h>
#include <string>

using namespace Zeni;

class Ship;

class Map_Object {

public:
	Map_Object() {}

	virtual const std::string get_type() const= 0;

	virtual void reset() {}

	virtual const Point3f &get_corner() const = 0;
	virtual const Vector3f &get_scale() const = 0;
	virtual const Quaternion &get_rotation() const = 0;

	virtual void step(const float &time_step) {}
	virtual void render() const = 0;
	virtual bool intersects(const Collision::Parallelepiped &p) const = 0;

	virtual void collide() {}
	virtual void collide_with_ship() {}
	virtual void collide_with_laser() {}
};