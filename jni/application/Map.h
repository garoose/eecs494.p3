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

	void render() const;
	Map_Object *intersects(const Collision::Parallelepiped &p) const;
	Map_Object *intersects(const Collision::Capsule &c) const;

	void write_to_file(const std::string &fname);
};