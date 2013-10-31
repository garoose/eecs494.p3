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

class Editor_Menu_State : public Widget_Gamestate {

public:
	static Map_Object *selected;

	Editor_Menu_State(Map_Object *selected_)
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true)
	{
		selected = selected_;

		m_widgets.lend_Widget(trans_x);
	}

	void on_key(const SDL_KeyboardEvent &event) override {
		switch (event.keysym.sym) {
		case SDLK_ESCAPE:
			get_Game().pop_state();
			break;

		default:
			Widget_Gamestate::on_key(event);
			break;
		}
	}

	class Translate_X : public Text_Box {
	public:
		Translate_X()
			: Text_Box(Point2f(200.0f, 100.0f), Point2f(620.0f, 100.0f), "system_36_800x600", "", get_Colors()["white"])
		{}

		void perform_logic() override {
			set_text(itoa(selected->get_center().x));
		}

	} trans_x;
};

class Editor : public Play_State {
	Map_Object *selected;
	Parameters parameter;
	Editor_Menu_State *m_menu;

	struct Controls {
		Controls() : key_read(false)
		{
			for (int i = 0; i < E_MAX; i++)
				key[i] = false;
		}

		void set_key(Event_Type e, bool value) {
			//key pressed
			if (value) {
				delay.set(0.0);
				delay.start();
				key_read = false;
			}

			key[e] = value;
		}

		bool get_key(Event_Type e) {
			if (!key_read && key[e]) {
				key_read = true;
				return key[e];
			}

			if (delay.seconds() > 0.5)
				return key[e];

			return false;
		}

		bool key[E_MAX];
		Chronometer<Time> delay;
		bool key_read;

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