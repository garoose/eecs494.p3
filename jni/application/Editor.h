#pragma once

#include <zenilib.h>
#include <string>

#include "Play_State.h"
#include "Map_Object.h"

using namespace Zeni;

enum Parameters {
	param_translate,
	param_scale,
	param_rotation,
};

class Editor : public Play_State {
	Map_Object *selected;
	Parameters parameter;

	struct Controls {
		Controls() : w(false), a(false), s(false), d(false), q(false), e(false) {}
		bool w;
		bool a;
		bool s;
		bool d;
		bool q;
		bool e;
	} m_controls;

public:
	Editor(const std::string &map_name_);
	~Editor();

	static Model *m_model;
	static unsigned long m_instance_count;

	void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action) override;

	void perform_logic() override;

	void adjust(const Vector3f &delta);
	void adjust_translate(const Vector3f &delta);
	void adjust_scale(const Vector3f &delta);
	void adjust_rotation(const float &angle, const Vector3f &ray);

	void render() override;
	void laser_collide_with_object(Map_Object *) override;
};