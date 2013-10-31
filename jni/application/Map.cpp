#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using std::string;
using std::cout; using std::endl;
using std::vector;

#include "Map.h"
#include "Wall.h"
#include "Floor1.h"
#include "Ship.h"
#include "Crate.h"
#include "Finish_Line.h"

Map::Map(const string &map_name_) {
	list.push_back(new Wall(Point3f(-256.0f, -128.0f, 0.0f), Vector3f(0.5f, 1.0f, 1.0f), Quaternion(Global::pi_over_two, 0.0f, 0.0f)));

	list.push_back(new Wall(Point3f(-256.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Wall(Point3f(-256.0f, -125.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Floor1(Point3f(-256.0f, -125.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Floor1(Point3f(-256.0f, -125.0f, 125.0f), Vector3f(1.0f, 1.0f, 1.0f)));

	list.push_back(new Wall(Point3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Wall(Point3f(0.0f, -125.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Floor1(Point3f(0.0f, -125.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Floor1(Point3f(0.0f, -125.0f, 125.0f), Vector3f(1.0f, 1.0f, 1.0f)));

	list.push_back(new Wall(Point3f(256.0f, 0.0f, 125.0f), Vector3f(1.0f, 1.0f, 1.0f), Quaternion(0.0f, Global::pi_over_two, 0.0f)));
	list.push_back(new Wall(Point3f(256.0f, -125.0f, 125.0f), Vector3f(1.0f, 1.0f, 1.0f), Quaternion(0.0f, Global::pi_over_two, 0.0f)));
	list.push_back(new Floor1(Point3f(251.0f, -125.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f), Quaternion(0.0f, Global::pi_over_two, 0.0f)));
	list.push_back(new Floor1(Point3f(256.0f, -125.0f, 125.0f), Vector3f(0.5f, 1.0f, 1.0f)));
	list.push_back(new Floor1(Point3f(390.0f, -125.0f, 125.0f), Vector3f(1.0f, 1.0f, 1.0f), Quaternion(0.0f, Global::pi_over_two, 0.0f)));

	list.push_back(new Crate(Point3f(0.0f, -100.0f, 70.0f), Vector3f(10.0f, 10.0f, 10.0f)));
}

Map::~Map() {
	for (auto it = list.begin(); it != list.end();) {
		auto item = (*it);

		it = list.erase(it);

		delete item;
	}
}

void Map::reset() {
	for (auto it = list.begin(); it != list.end(); ++it)
		(*it)->reset();
}

void Map::step(const float &time_step) {
	for (auto it = list.begin(); it != list.end(); ++it)
		(*it)->step(time_step);
}

void Map::render() const {
	for (auto it = list.begin(); it != list.end(); ++it) {
		(*it)->render();
	}
}

Map_Object *Map::intersects(const Collision::Parallelepiped &p) const {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it)->intersects(p))
			return (*it);
	}

	return nullptr;
}

Map_Object *Map::get_next(const Map_Object *o) const {
	if (o) {
		auto it = std::find(list.begin(), list.end(), o);
		if (it == list.end())
			return nullptr;
		auto next = it++;
		if (next != list.end())
			return (*next);
	}

	return (*list.begin());
}

Map_Object *Map::get_prev(const Map_Object *o) const {
	if (o) {
		auto it = std::find(list.begin(), list.end(), o);
		if (it == list.end())
			return nullptr;
		auto prev = it--;
		if (prev != list.end())
			return (*prev);
	}

	return (*list.begin());
}

void Map::write_to_file(const string &fname) {
	std::ofstream file(fname);
	string line;

	for (auto it = list.begin(); it != list.end(); ++it) {
		auto wall = (*it);
		auto corner = wall->get_corner();
		auto scale = wall->get_scale();
		auto rotation = wall->get_rotation().get_rotation();
		auto axis = rotation.first;
		auto angle = rotation.second;
		file << "Wall " << corner.x << " " << corner.y << " " << corner.z
			<< " " << scale.x << " " << scale.y << " " << scale.z << " "
			<< axis.x << " " << axis.y << " " << axis.z << " " << angle;
	}

	file.close();
}

void Map::read_from_file(const string &fname) {

}