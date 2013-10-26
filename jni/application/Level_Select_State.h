#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Map.h"
#include "Play_State.h"

using namespace Zeni;

class Level_Select_State : public Widget_Gamestate {
	std::vector<std::string> levels;
	static std::string selected;

public:
	Level_Select_State()
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true)	
	{
		levels.push_back("level1.txt");
		selected = levels.back();

		m_widgets.lend_Widget(play_button);
		m_widgets.lend_Widget(edit_button);
	}

	class Play_Button : public Text_Button {
	public:
		Play_Button()
		: Text_Button(Point2f(250.0f, 500.0f), Point2f(350.0f, 550.0f), "system_36_800x600", "Play")
		{}

		void on_accept() {
			Play_State s(selected);
			//get_Game().push_state(s);
		}

	} play_button;

	class Edit_Button : public Text_Button {
	public:
		Edit_Button()
		: Text_Button(Point2f(450.0f, 500.0f), Point2f(550.0f, 550.0f), "system_36_800x600", "Edit")
		{}


	} edit_button;

};