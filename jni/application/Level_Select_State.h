#pragma once

#include <zenilib.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Map.h"
#include "Play_State.h"
#include "Editor.h"

using namespace Zeni;

struct Level {
	Level(const std::string &name_, const std::string &fname_)
	: name(name_), fname(fname_)
	{}

	std::string name;
	std::string fname;
};

static std::vector<Level> levels;
static std::vector<Level>::iterator selected_map;
static Play_State *m_play_state;

static const Vector2f button_size = Vector2f(120.0f, 50.0f);
static const float row[3] = { 200.0f, 320.0f, 390.0f };
static const float col[2] = { 280.0f, 420.0f };

class My_Button : public Text_Button {
public:
	My_Button(const Point2f &corner_, const Vector2f &size_, const String &text_)
		: Text_Button(corner_, corner_ + size_, "system_36_800x600", text_)
	{
	}
};

class Level_Select_State : public Widget_Gamestate {

public:
	Level_Select_State()
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true)
	{
		auto it = levels.begin();
		load_levels();
		selected_map = levels.begin();

		m_play_state = nullptr;

		m_widgets.lend_Widget(map_selection);
		//map_selection.lend_BG_Renderer(new Widget_Renderer_Color(Color(0.0f, 0.0f, 0.0f, 0.0f)));
		map_selection.lend_BG_Renderer(new Widget_Renderer_Color(Color(0.3f, 0.0f, 0.6f, 1.0f)));
		map_selection.set_justify(ZENI_CENTER);

		m_widgets.lend_Widget(play_button);
		play_button.give_Renderer(new Widget_Renderer_Tricolor(
			Color(0.8f, 0.0f, 1.0f, 0.4f),
			Color(0.8f, 0.6f, 1.0f, 1.0f),
			Color(0.8f, 0.0f, 1.0f, 0.8f),
			get_Colors()["default_button_text_normal"],
			get_Colors()["default_button_text_clicked"],
			get_Colors()["default_button_text_hovered_strayed"]));

		m_widgets.lend_Widget(prev_button);
		m_widgets.lend_Widget(next_button);
		m_widgets.lend_Widget(back_button);
		m_widgets.lend_Widget(edit_button);
	}

	void on_uncover() override {
		//delete m_play_state;
		m_play_state = nullptr;
		//get_Game().pop_state();
	}

	void on_key(const SDL_KeyboardEvent &event) override {
		if (event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
			get_Game().pop_state();
		else
			Widget_Gamestate::on_key(event);
	}

	void load_levels() {
		std::ifstream in("config/levels.txt");
		std::string line;

		while (getline(in, line)) {
			int last_space = line.find_last_of(' ');
			std::string name = line.substr(0, last_space);
			std::string fname = line.substr(last_space + 1, line.size() - last_space);
			levels.push_back(Level(name, fname));
		}
	}

	class Map_Selection : public Text_Box {
	public:
		Map_Selection()
			: Text_Box(Point2f(200.0f, row[0]), Point2f(620.0f, row[0] + 100.0f), "system_36_800x600", "", get_Colors()["white"])
		{}

		void perform_logic() override {
			set_text(String(selected_map->name));
		}

	} map_selection;
	
	class Play_Button : public My_Button {
	public:
		Play_Button()
			: My_Button(Point2f(col[0], row[1]), Vector2f(button_size.x * 2.0f + 20.0f, button_size.y), "Play")
		{}

		void on_accept() override {
			m_play_state = new Play_State(selected_map->fname);
			get_Game().push_state(m_play_state);
		}

	} play_button;

	class Prev_Button : public My_Button {
	public:
		Prev_Button()
			: My_Button(Point2f(col[0], row[2]), button_size, "Previous")
		{}

		void on_accept() override {
			if (selected_map == levels.begin())
				selected_map = levels.end();
			selected_map--;
		}

	} prev_button;

	class Next_Button : public My_Button {
	public:
		Next_Button()
			: My_Button(Point2f(col[1], row[2]), button_size, "Next")
		{}

		void on_accept() override {
			selected_map++;
			if (selected_map == levels.end())
				selected_map = levels.begin();
		}

	} next_button;

	class Back_Button : public My_Button {
	public:
		Back_Button()
			: My_Button(Point2f(20.0f, 500.0f), button_size, "Back")
		{}

		void on_accept() override {
			get_Game().pop_state();
		}

	} back_button;

	class Edit_Button : public My_Button {
	public:
		Edit_Button()
			: My_Button(Point2f(700.0f, 500.0f), button_size, "Edit")
		{}

		void on_accept() override {
			get_Game().push_state(new Editor(selected_map->fname));
		}

	} edit_button;

	void render() override {
		render_image("stars", Point2f(), Point2f(800, 600));

		Widget_Gamestate::render();

		render_image("level_select", Point2f(150.0f, 25.0f), Point2f(650.0f, 225.0f));
	}

};