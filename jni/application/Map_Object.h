#pragma once

#include <zenilib.h>

using namespace Zeni;

class Ship;

class Map_Object {

public:
	Map_Object() {}

	virtual void render() const = 0;
	virtual bool intersects(const Collision::Parallelepiped &p) const = 0;
	virtual bool intersects(const Collision::Capsule &c) const { return false; }
	//virtual const Vector3f &get_surface() const = 0;

	virtual void collide() {}
};