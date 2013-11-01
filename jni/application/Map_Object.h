#pragma once

#include <zenilib.h>
#include <string>

using namespace Zeni;

class Ship;

class Map_Object {

public:
	Map_Object() {}
	virtual ~Map_Object() { }

	virtual const std::string get_type() const = 0;

	virtual void reset() {}

	virtual const Point3f &get_corner() const = 0;
	virtual const Vector3f &get_scale() const = 0;
	virtual const Quaternion &get_rotation() const = 0;
	virtual const Point3f get_center() const = 0;

	virtual void set_translate(const Point3f &position) = 0;
	virtual void set_scale(const Vector3f &scale) = 0;
	virtual void set_rotate(const float &angle, const Vector3f &ray) = 0;

	virtual void adjust_pitch(const float &phi) = 0;
	virtual void adjust_roll(const float &rho) = 0;
	virtual void adjust_yaw(const float &theta) = 0;

	virtual void step(const float &) {}
	virtual void render() const = 0;
	virtual bool intersects(const Collision::Parallelepiped &p) const = 0;

	virtual void collide() {}
	virtual void collide_with_ship() {}
	virtual void collide_with_laser() {}
};