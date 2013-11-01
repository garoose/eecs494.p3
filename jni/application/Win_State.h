#pragma once

#include <zenilib.h>

using namespace Zeni;

class Win_State : public Widget_Gamestate {
public:
	Win_State(Time_HQ::Second_Type time_)
		: Widget_Gamestate(std::make_pair(Point2f(), Point2f(800.0f, 600.0f)), true),
		time_taken(time_)
	{
		//win_text.set_justify(ZENI_CENTER);
		//m_widgets.lend_Widget(win_text);
		m_widgets.lend_Widget(time_taken);
		m_widgets.lend_Widget(restart_button);
		m_widgets.lend_Widget(quit_button);
	}

private:

	void render() override {
		render_image("stars", Point2f(), Point2f(800, 600));

		Widget_Gamestate::render();

		render_image("finish_msg", Point2f(50.0f, 25.0f), Point2f(750.0f, 225.0f));
	}

	class Time_Taken : public Text_Box {
	public:
		Time_Taken(Time_HQ::Second_Type time_)
			: Text_Box(Point2f(100.0f, 250.0f), Point2f(500.0f, 325.0f), "title", "", get_Colors()["green"])
		{
			char buffer[1000];
			sprintf(buffer, "%.2f", time_);
			String sbuffer(buffer);
			set_text("Time:" + sbuffer);
		}
	} time_taken;

	class Restart : public Text_Button {
	public:
		Restart()
			: Text_Button(Point2f(50.0f, 500.0f), Point2f(250.0f, 550.0f), "system_36_800x600", "Restart")
		{}

		void on_accept() {
			get_Game().pop_state();
		}
	} restart_button;

	class Quit : public Text_Button {
	public:
		Quit()
			: Text_Button(Point2f(550.0f, 500.0f), Point2f(750.0f, 550.0f), "system_36_800x600", "Quit")
		{}

		void on_accept() {
			get_Game().pop_state();
			get_Game().pop_state();
		}
	} quit_button;
};