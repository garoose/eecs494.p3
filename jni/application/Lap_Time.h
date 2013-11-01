#pragma once

#include <zenilib.h>

using namespace Zeni;

class Lap_Time {
	Chronometer<Time_HQ> m_time;

public:
	Lap_Time();
	~Lap_Time();

	void reset();
	
	const Time_HQ::Second_Type get_time() { return m_time.seconds(); }
	
	void pause();
	void unpause();
	void lap();

	void render() const;
};