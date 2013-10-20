#pragma once

#include <zenilib.h>

using namespace Zeni;

class Ship;

class Map_Object {

public:
	Map_Object() {}

	virtual void render() const = 0;
	virtual bool is_intersecting(const Ship &s) const = 0;
};