#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::string;
using std::cout; using std::endl;

#include "Map.h"
#include "Wall.h"
#include "Floor1.h"
#include "Ship.h"
#include "Finish_Line.h"

using std::vector;

Map::Map(const string &map_name_) {
		/*list.push_back(new Wall(Point3f(-100.0f, 12.0f, -25.0f), Vector3f(600.0f, 5.0f, 60.0f))); //left side
		list.push_back(new Wall(Point3f(-100.0f, 12.0f, -25.0f), Vector3f(660.0f, -60.0f, 5.0f))); //bottom
		list.push_back(new Wall(Point3f(-100.0f, -48.0f, -25.0f), Vector3f(660.0f, 5.0f, 60.0f))); //right side
		list.push_back(new Wall(Point3f(-100.0f, 12.0f, 35.0f), Vector3f(660.0f, -60.0f, 5.0f))); //top

		list.push_back(new Wall(Point3f(560.0f, -48.0f, -25.0f), Vector3f(5.0f, 260.0f, 60.0f))); //left side
		list.push_back(new Wall(Point3f(560.0f, 12.0f, -25.0f), Vector3f(-60.0f, 200.0f, 5.0f))); //bottom
		list.push_back(new Wall(Point3f(500.0f, 12.0f, -25.0f), Vector3f(5.0f, 200.0f, 60.0f))); //right side
		list.push_back(new Wall(Point3f(560.0f, 12.0f, 35.0f), Vector3f(-60.0f, 260.0f, 5.0f))); //top

		list.push_back(new Wall(Point3f(560.0f, 212.0f, 35.0f), Vector3f(5.0f, 120.0f, -400.0f))); //left side
		list.push_back(new Wall(Point3f(560.0f, 212.0f, -25.0f), Vector3f(-60.0f, 5.0f, -400.0f))); //bottom
		list.push_back(new Wall(Point3f(500.0f, 212.0f, 35.0f), Vector3f(5.0f, 120.0f, -400.0f))); //right side
		list.push_back(new Wall(Point3f(560.0f, 272.0f, 35.0f), Vector3f(-60.0f, 5.0f, -400.0f), Quaternion(0.0f, 0.0f, 0.13f))); //top

		list.push_back(new Wall(Point3f(560.0f, 212.0f, -365.0f), Vector3f(5.0f, 520.0f, -60.0f))); //left side
		list.push_back(new Wall(Point3f(560.0f, 212.0f, -425.0f), Vector3f(-60.0f, 520.0f, 5.0f))); //bottom
		list.push_back(new Wall(Point3f(500.0f, 212.0f, -365.0f), Vector3f(5.0f, 520.0f, -60.0f))); //right side
		list.push_back(new Wall(Point3f(560.0f, 332.0f, -365.0f), Vector3f(-60.0f, 460.0f, 5.0f))); //top*/

	list.push_back(new Wall(Point3f(0.0f, 0.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
	list.push_back(new Floor1(Point3f(0.0f, 100.0f, 0.0f), Vector3f(1.0f, 1.0f, 1.0f)));
}

Map::~Map() {
	for (auto it = list.begin(); it != list.end();) {
		auto item = (*it);

		it = list.erase(it);

		delete item;
	}
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

void Map::write_to_file(const string &fname) {
	std::ofstream file(fname);
	string line;

	for (auto it = list.begin(); it != list.end(); ++it) {
		auto wall = (*it);
		auto corner = wall->get_corner();
		auto scale = wall->get_scale();
		auto rotation = wall->get_rotation();
		file << "Wall " << corner.x << " " << corner.y << " " << corner.z << " " << scale.x << " " << scale.y << " " << scale.z << " ";
	}
}