#include "Tunnel.h"
#include "Wall.h"

using std::vector;

Tunnel::Tunnel(const Point3f &corner, const Vector3f &length, const float &height, const float &width) {
	walls.push_back(new Wall(Point3f(-100.0f, 12.0f, -25.0f), Vector3f(600.0f, 5.0f, 60.0f))); //left side
	walls.push_back(new Wall(Point3f(-100.0f, 12.0f, -25.0f), Vector3f(660.0f, -60.0f, 5.0f))); //bottom
	walls.push_back(new Wall(Point3f(-100.0f, -48.0f, -25.0f), Vector3f(660.0f, 5.0f, 60.0f))); //right side
	walls.push_back(new Wall(Point3f(-100.0f, 12.0f, 35.0f), Vector3f(660.0f, -60.0f, 5.0f))); //top
}

Tunnel::~Tunnel() {
	for (auto it = walls.begin(); it != walls.end();) {
		auto item = (*it);
		
		walls.erase(it);

		delete item;
	}
}