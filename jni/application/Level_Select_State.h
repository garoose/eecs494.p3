#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Map.h"
#include "Play_State.h"

using namespace Zeni;

static std::vector<std::string> levels;
static std::vector<std::string>::iterator selected;
static Play_State *m_play_state;
//static std::string selected;

static const Vector2f button_size = Vector2f(120.0f, 50.0f);
static const float row[3] = { 200.0f, 320.0f, 390.0f };
static const float col[2] = { 280.0f, 420.0f };

static class My_Button : public Text_Button {
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
		levels.push_back("level1.txt");
		levels.push_back("level2.txt");
		selected = levels.begin();

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
		delete m_play_state;

		get_Game().pop_state();
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

	class Map_Selection : public Text_Box {
	public:
		Map_Selection()
			: Text_Box(Point2f(200.0f, row[0]), Point2f(620.0f, row[0] + 100.0f), "system_36_800x600", "", get_Colors()["white"])
		{}

		void perform_logic() override {
			set_text(String(*selected));
		}

	} map_selection;
	
	class Play_Button : public My_Button {
	public:
		Play_Button()
			: My_Button(Point2f(col[0], row[1]), Vector2f(button_size.x * 2.0f + 20.0f, button_size.y), "Play")
		{}

		void on_accept() override {
			Play_State *s = new Play_State(*selected);
			get_Game().push_state(s);
		}

	} play_button;

	class Prev_Button : public My_Button {
	public:
		Prev_Button()
			: My_Button(Point2f(col[0], row[2]), button_size, "Previous")
		{}

		void on_accept() override {
			if (selected == levels.begin())
				selected = levels.end();
			selected--;
		}

	} prev_button;

	class Next_Button : public My_Button {
	public:
		Next_Button()
			: My_Button(Point2f(col[1], row[2]), button_size, "Next")
		{}

		void on_accept() override {
			selected++;
			if (selected == levels.end())
				selected = levels.begin();
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

	} edit_button;

	void render() override {
		render_image("stars", Point2f(), Point2f(get_Window().get_width() / 2.0f, get_Window().get_height()));

		Widget_Gamestate::render();

		render_image("level_select", Point2f(150.0f, 25.0f), Point2f(650.0f, 225.0f));
	}

};