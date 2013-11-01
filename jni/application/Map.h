#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Wall.h"
#include "Map_Object.h"
#include "Finish_Line.h"
#include "Map_Object_Factory.h"

using namespace Zeni;

class Map {
	std::string file_name;
	std::vector<Map_Object *> list;
	Finish_Line *m_finish;

	Map_Object_Factory factory;

public:
	Map(const std::string &file_name_);
	~Map();

	void reset();

	void step(const float &time_step);
	void render() const;
	Map_Object *intersects(const Collision::Parallelepiped &p) const;
	Map_Object *intersects(const Collision::Capsule &c) const;

	Finish_Line *get_finish() { return m_finish; }

	Map_Object *get_next(const Map_Object *o) const;
	Map_Object *get_prev(const Map_Object *o) const;

	void add_item(Map_Object *o);
	Map_Object *remove_item(Map_Object *o);

	void load();
	void save();

	void write_to_file(const std::string &fname);
	void read_from_file(const std::string &fname);
	Map_Object *parse_line(const std::string &line);
	void print_object(const Map_Object *o, std::ofstream &file);

private:
	std::vector<std::string> split_string(std::string line);
};