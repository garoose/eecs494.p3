#pragma once

#include <zenilib.h>
#include <vector>

using namespace Zeni;

class Wall;

class Tunnel {
	std::vector<Wall *> walls;

public:
	Tunnel(const Point3f &corner, const Vector3f &length, const float &height, const float &width);
	~Tunnel();

};