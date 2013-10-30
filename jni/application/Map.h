#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Wall.h"
#include "Map_Object.h"

using namespace Zeni;

class Map {

public:
	std::vector<Map_Object *> list;

	Map(const std::string &map_name_);
	~Map();

	void reset();

	void step(const float &time_step);
	void render() const;
	Map_Object *intersects(const Collision::Parallelepiped &p) const;
	Map_Object *intersects(const Collision::Capsule &c) const;

	Map_Object *get_next(const Map_Object *o) const;
	Map_Object *get_prev(const Map_Object *o) const;

	void write_to_file(const std::string &fname);
};