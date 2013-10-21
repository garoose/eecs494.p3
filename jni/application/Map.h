#pragma once

#include <zenilib.h>
#include <vector>

#include "Wall.h"
#include "Map_Object.h"

using namespace Zeni;

class Ship;

class Map {

public:
	std::vector<Map_Object *> list;

	Map();
	~Map();

	void render() const;
	Map_Object *intersects(const Ship &s) const;
};