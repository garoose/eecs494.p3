#pragma once

#include <zenilib.h>

#include "Editor.h"

using namespace Zeni;

class Editor_Menu_State : public Widget_Gamestate {

public:
	Editor_Menu_State(Editor *editor_)
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true),
		trans_x(editor_),
		trans_y(editor_),
		trans_z(editor_),
		scale_x(editor_),
		scale_y(editor_),
		scale_z(editor_),
		save_button(editor_),
		delete_button(editor_)
	{
		m_widgets.lend_Widget(trans);
		m_widgets.lend_Widget(trans_x);
		m_widgets.lend_Widget(trans_y);
		m_widgets.lend_Widget(trans_z);

		m_widgets.lend_Widget(scale);
		m_widgets.lend_Widget(scale_x);
		m_widgets.lend_Widget(scale_y);
		m_widgets.lend_Widget(scale_z);

		m_widgets.lend_Widget(delete_button);
		m_widgets.lend_Widget(save_button);
		m_widgets.lend_Widget(close_button);
		m_widgets.lend_Widget(quit_button);
	}

	void on_key(const SDL_KeyboardEvent &event) override {
		if (event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
			get_Game().pop_state();
		else
			Widget_Gamestate::on_key(event);
	}

	class Translate : public Text_Box {
	public:
		Translate()
			: Text_Box(Point2f(50.0f, 100.0f), Point2f(180.0f, 150.0f), "system_36_800x600", "Translate:", get_Colors()["white"])
		{}
	} trans;

	class Translate_X : public Text_Box {
		Map_Object *m_selected;
	public:
		Translate_X(Editor *e)
			: Text_Box(Point2f(200.0f, 100.0f), Point2f(360.0f, 150.0f), "system_36_800x600", "transx", get_Colors()["white"], true),
			m_selected(e->get_selected())
		{
			if (m_selected)
				set_text(ftoa(m_selected->get_corner().x));
		}

		void on_change() override {
			if (m_selected) {
				Point3f corner = m_selected->get_corner();
				m_selected->set_translate(Vector3f(atof(get_text().c_str()), corner.y, corner.z));
			}
		}
	} trans_x;

	class Translate_Y : public Text_Box {
		Map_Object *m_selected;
	public:
		Translate_Y(Editor *e)
			: Text_Box(Point2f(380.0f, 100.0f), Point2f(550.0f, 150.0f), "system_36_800x600", "transy", get_Colors()["white"], true),
			m_selected(e->get_selected())
		{
			if (m_selected)
				set_text(ftoa(m_selected->get_corner().y));
			else
				set_editable(false);
		}

		void on_change() override {
			if (m_selected) {
				Point3f corner = m_selected->get_corner();
				m_selected->set_translate(Vector3f(corner.x, atof(get_text().c_str()), corner.z));
			}
		}
	} trans_y;

	class Translate_Z : public Text_Box {
		Map_Object *m_selected;
	public:
		Translate_Z(Editor *e)
			: Text_Box(Point2f(570.0f, 100.0f), Point2f(730.0f, 150.0f), "system_36_800x600", "transz", get_Colors()["white"], true),
			m_selected(e->get_selected())
		{
			if (m_selected)
				set_text(ftoa(m_selected->get_corner().z));
		}

		void on_change() override {
			if (m_selected) {
				Point3f corner = m_selected->get_corner();
				m_selected->set_translate(Vector3f(corner.x, corner.y, atof(get_text().c_str())));
			}
		}
	} trans_z;

	class Scale : public Text_Box {
	public:
		Scale()
			: Text_Box(Point2f(50.0f, 200.0f), Point2f(180.0f, 250.0f), "system_36_800x600", "Scale:", get_Colors()["white"])
		{}
	} scale;

	class Scale_X : public Text_Box {
		Map_Object *m_selected;
	public:
		Scale_X(Editor *e)
			: Text_Box(Point2f(200.0f, 200.0f), Point2f(360.0f, 250.0f), "system_36_800x600", "scalex", get_Colors()["white"], true),
			m_selected(e->get_selected())
		{
			if (m_selected)
				set_text(ftoa(m_selected->get_scale().x));
		}

		void on_change() override {
			if (m_selected) {
				Vector3f scale = m_selected->get_scale();
				m_selected->set_scale(Vector3f(atof(get_text().c_str()), scale.y, scale.z));
			}
		}
	} scale_x;

	class Scale_Y : public Text_Box {
		Map_Object *m_selected;
	public:
		Scale_Y(Editor *e)
			: Text_Box(Point2f(380.0f, 200.0f), Point2f(550.0f, 250.0f), "system_36_800x600", "scaley", get_Colors()["white"], true),
			m_selected(e->get_selected())
		{
			if (m_selected)
				set_text(ftoa(m_selected->get_scale().y));
			else
				set_editable(false);
		}

		void on_change() override {
			if (m_selected) {
				Vector3f scale = m_selected->get_scale();
				m_selected->set_scale(Vector3f(scale.x, atof(get_text().c_str()), scale.z));
			}
		}
	} scale_y;

	class Scale_Z : public Text_Box {
		Map_Object *m_selected;
	public:
		Scale_Z(Editor *e)
			: Text_Box(Point2f(570.0f, 200.0f), Point2f(730.0f, 250.0f), "system_36_800x600", "scalez", get_Colors()["white"], true),
			m_selected(e->get_selected())
		{
			if (m_selected)
				set_text(ftoa(m_selected->get_scale().z));
		}

		void on_change() override {
			if (m_selected) {
				Vector3f scale = m_selected->get_scale();
				m_selected->set_scale(Vector3f(scale.x, scale.y, atof(get_text().c_str())));
			}
		}
	} scale_z;

	class Delete : public Text_Button {
		Editor *m_editor;
	public:
		Delete(Editor *e)
			: Text_Button(Point2f(150.0f, 300.0f), Point2f(250.0f, 350.0f), "system_36_800x600", "Delete")
		{}

		void on_accept() {
			m_editor->remove_object(m_editor->get_selected());
			get_Game().pop_state();
		}
	} delete_button;

	class Save : public Text_Button {
		Editor *m_editor;
	public:
		Save(Editor *e)
			: Text_Button(Point2f(300.0f, 500.0f), Point2f(500.0f, 550.0f), "system_36_800x600", "Save Map"),
			m_editor(e)
		{}

		void on_accept() {
			m_editor->save_map();
		}
	} save_button;

	class Close : public Text_Button {
	public:
		Close()
			: Text_Button(Point2f(50.0f, 500.0f), Point2f(150.0f, 550.0f), "system_36_800x600", "Close")
		{}

		void on_accept() {
			get_Game().pop_state();
		}
	} close_button;

	class Quit : public Text_Button {
	public:
		Quit()
			: Text_Button(Point2f(650.0f, 500.0f), Point2f(750.0f, 550.0f), "system_36_800x600", "Quit")
		{}

		void on_accept() {
			get_Game().pop_state();
			get_Game().pop_state();
		}
	} quit_button;
};
