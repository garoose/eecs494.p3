#include "Lap_Time.h"

#include <cstdio>

Lap_Time::Lap_Time() {
	m_time.start();
}

Lap_Time::~Lap_Time() {
	m_time.stop();
}

void Lap_Time::reset() {
	m_time.set(0.0f);
	unpause();
}

void Lap_Time::pause() {
	m_time.pause_all();
}

void Lap_Time::unpause() {
	m_time.unpause_all();
}

void Lap_Time::render() const {
	Video &vr = get_Video();

	Font &fr = get_Fonts()["title"];

	char buffer[1000];
	sprintf(buffer, "%.2f", m_time.seconds());

	//String time_s = ftoa(m_time.seconds());

	fr.render_text(
		buffer,
		Point2f(get_Window().get_size().x - 100.0f, 100.0f - 0.5f * fr.get_text_height()),
		get_Colors()["title_text"],
		ZENI_CENTER);
}