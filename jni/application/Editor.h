#pragma once

#include <zenilib.h>
#include <string>

#include "Play_State.h"
#include "Map_Object.h"
#include "Map_Object_Factory.h"

using namespace Zeni;

enum Parameters {
	param_translate,
	param_scale,
	param_rotation,
};

class Object_Creator;
class Editor_Menu_State;

class Editor : public Play_State {
	Map_Object *selected;
	Parameters parameter;
	Editor_Menu_State *m_menu;
	Object_Creator *m_creator;

	Map_Object_Factory factory;

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

	Map_Object *get_selected() { return selected; }
	void set_selected(Map_Object *m) { selected = m; }

	void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action) override;

	void perform_logic() override;

	void adjust(const Vector3f &delta);
	void adjust_translate(const Vector3f &delta);
	void adjust_scale(const Vector3f &delta);
	void adjust_rotation(const Vector3f &delta);

	const Point3f &get_player_create_position() { return m_player.get_position() + m_player.get_forward() * m_player.get_size().x; }

	void render_3d() override;
	void laser_collide_with_object(Map_Object *) override;

	void insert_object(Map_Object *mo) {
		m_map.add_item(mo);
	}

	void save_map() {
		m_map.save();
	}
};