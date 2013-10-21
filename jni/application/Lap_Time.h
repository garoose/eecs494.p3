#pragma once

#include <zenilib.h>

using namespace Zeni;

class Lap_Time {
	Chronometer<Time_HQ> m_time;

public:
	Lap_Time();
	~Lap_Time();

	void reset();
	void pause();
	void unpase();
	void lap();

	void render() const;
};