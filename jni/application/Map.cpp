#include "Map.h"
#include "Wall.h"
#include "Ship.h"

using std::vector;

Map::Map() {
		list.push_back(new Wall(Point3f(-100.0f, 12.0f, -25.0f), Vector3f(600.0f, 5.0f, 60.0f))); //left side
		list.push_back(new Wall(Point3f(-100.0f, 12.0f, -25.0f), Vector3f(660.0f, -60.0f, 5.0f))); //bottom
		list.push_back(new Wall(Point3f(-100.0f, -48.0f, -25.0f), Vector3f(660.0f, 5.0f, 60.0f))); //right side
		list.push_back(new Wall(Point3f(-100.0f, 12.0f, 35.0f), Vector3f(660.0f, -60.0f, 5.0f))); //top

		list.push_back(new Wall(Point3f(560.0f, -48.0f, -25.0f), Vector3f(5.0f, 260.0f, 60.0f))); //left side
		list.push_back(new Wall(Point3f(560.0f, 12.0f, -25.0f), Vector3f(-60.0f, 200.0f, 5.0f))); //bottom
		list.push_back(new Wall(Point3f(500.0f, 12.0f, -25.0f), Vector3f(5.0f, 200.0f, 60.0f))); //right side
		list.push_back(new Wall(Point3f(560.0f, 12.0f, 35.0f), Vector3f(-60.0f, 200.0f, 5.0f))); //top
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

Map_Object *Map::intersects(const Ship &s) const {
	for (auto it = list.begin(); it != list.end(); ++it) {
		if ((*it)->intersects(s))
			return (*it);
	}

	return nullptr;
}