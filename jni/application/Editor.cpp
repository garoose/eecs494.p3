#include "Editor.h"

class Object_Creator : public Widget_Gamestate {
	static Editor *editor;
	static Map_Object_Factory factory;
	static String type;

public:
	Object_Creator(Editor *e)
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true)
	{
		editor = e;

		m_widgets.lend_Widget(type_text);
		m_widgets.lend_Widget(create_type);
		m_widgets.lend_Widget(create_button);

		create_type.set_focus(true);
	}

	class Type_Text : public Text_Box {
	public:
		Type_Text()
			: Text_Box(Point2f(50.0f, 100.0f), Point2f(250.0f, 150.0f), "system_36_800x600", "Enter Type:", get_Colors()["white"])
		{}
	} type_text;

	class Type_Box : public Text_Box {
	public:
		Type_Box()
			: Text_Box(Point2f(300.0f, 100.0f), Point2f(620.0f, 150.0f), "system_36_800x600", "", get_Colors()["white"], true)
		{
		}

		void on_change() {
			type = get_text();
		}

		void on_key(const SDL_Keysym &keysym, const bool &down) override {
			if (keysym.sym == SDLK_RETURN && down)
				create_and_close();
			else
				Text_Box::on_key(keysym, down);
		}
	} create_type;

	class Create : public Text_Button {
	public:
		Create()
			: Text_Button(Point2f(300.0f, 300.0f), Point2f(400.0f, 350.0f), "system_36_800x600", "Create")
		{}

		void on_accept() {
			create_and_close();
		}
	} create_button;

	static void create_and_close() {
		Map_Object *m = factory.make(type, editor->get_player_create_position());
		if (m) {
			editor->insert_object(m);
			editor->set_selected(m);
		}

		get_Game().pop_state();
	}

	void on_key(const SDL_KeyboardEvent &event) override {
		if (event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
			get_Game().pop_state();
		else
			Widget_Gamestate::on_key(event);
	}
};

Editor *Object_Creator::editor;
Map_Object_Factory Object_Creator::factory;
String Object_Creator::type;

class Editor_Menu_State : public Widget_Gamestate {

public:
	static Editor *m_editor;
	static Map_Object *m_selected;

	Editor_Menu_State(Editor *editor_)
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true)
	{
		m_editor = editor_;
		m_selected = m_editor->get_selected();

		m_widgets.lend_Widget(trans);
		m_widgets.lend_Widget(trans_x);
		m_widgets.lend_Widget(trans_y);
		m_widgets.lend_Widget(trans_z);

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
	public:
		Translate_X()
			: Text_Box(Point2f(200.0f, 100.0f), Point2f(350.0f, 150.0f), "system_36_800x600", "transx", get_Colors()["white"], true)
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
	public:
		Translate_Y()
			: Text_Box(Point2f(370.0f, 100.0f), Point2f(530.0f, 150.0f), "system_36_800x600", "transy", get_Colors()["white"], true)
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
	public:
		Translate_Z()
		: Text_Box(Point2f(550.0f, 100.0f), Point2f(690.0f, 150.0f), "system_36_800x600", "transz", get_Colors()["white"], true)
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

	class Save : public Text_Button {
	public:
		Save()
		: Text_Button(Point2f(400.0f, 500.0f), Point2f(600.0f, 550.0f), "system_36_800x600", "Save Map")
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
		: Text_Button(Point2f(200.0f, 500.0f), Point2f(300.0f, 550.0f), "system_36_800x600", "Quit")
		{}

		void on_accept() {
			get_Game().pop_state();
			get_Game().pop_state();
		}
	} quit_button;
};

Editor *Editor_Menu_State::m_editor = nullptr;
Map_Object *Editor_Menu_State::m_selected = nullptr;



Editor::Editor(const std::string &map_name_)
: Play_State(map_name_),
parameter(param_translate),
selected(nullptr)
{
	if (!m_instance_count) {
		m_model = new Model("models/crate.3ds");
	}
	++m_instance_count;

	m_menu = new Editor_Menu_State(this);
	m_creator = new Object_Creator(this);
}

Editor::~Editor() {
	if (!--m_instance_count) {
		delete m_model;
		m_model = nullptr;
	}

	delete m_menu;
	delete m_creator;
}

void Editor::on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action) {
	switch (action) {
	case E_W:
	case E_A:
	case E_S:
	case E_D:
	case E_Q:
	case E_E:
		m_controls.set_key(Event_Type(action), confidence ? true : false);
		break;

	case E_Z:
		if (confidence == 1.0f)
			parameter = param_translate;
		break;
	case E_X:
		if (confidence == 1.0f)
			parameter = param_scale;
		break;
	case E_C:
		if (confidence == 1.0f)
			parameter = param_rotation;
		break;
	case E_N:
		if (confidence == 1.0f)
			get_Game().push_state(new Object_Creator(this));
		break;
	case E_RIGHT:
		if (confidence == 1.0f)
			selected = m_map.get_next(selected);
		break;
	case E_LEFT:
		if (confidence == 1.0f)
			selected = m_map.get_prev(selected);
		break;

	case E_EXIT:
		if (confidence == 1.0f)
			get_Game().push_state(new Editor_Menu_State(this));
		break;

	default:
		Play_State::on_event(id, confidence, action);
	}
}

void Editor::perform_logic() {
	if (m_controls.get_key(E_W))
		adjust(Vector3f(1.0f, 0.0f, 0.0f));
	if (m_controls.get_key(E_A))
		adjust(Vector3f(0.0f, 1.0f, 0.0f));
	if (m_controls.get_key(E_S))
		adjust(Vector3f(-1.0f, 0.0f, 0.0f));
	if (m_controls.get_key(E_D))
		adjust(Vector3f(0.0f, -1.0f, 0.0f));
	if (m_controls.get_key(E_Q)) {
			adjust(Vector3f(0.0f, 0.0f, -1.0f));
	}
	if (m_controls.get_key(E_E)) {
			adjust(Vector3f(0.0f, 0.0f, 1.0f));
	}

	Play_State::perform_logic();
}

void Editor::adjust(const Vector3f &delta) {
	switch (parameter) {
	case param_translate:
		adjust_translate(delta / 2.0f);
		break;
	case param_scale:
		adjust_scale(delta / 5.0f);
		break;
	case param_rotation:
		adjust_rotation(delta);
		break;
	}
}

void Editor::adjust_translate(const Vector3f &delta) {
	if (selected) {
		selected->set_translate(selected->get_corner() + delta);
	}
}

void Editor::adjust_scale(const Vector3f &delta) {
	if (selected) {
		selected->set_scale(selected->get_scale() + delta);
	}
}

void Editor::adjust_rotation(const Vector3f &delta) {
	if (selected) {
		selected->adjust_pitch(delta.x * Global::pi / 4.0f);
		selected->adjust_yaw(delta.y * Global::pi / 4.0f);
		selected->adjust_roll(delta.z * Global::pi / 4.0f);
	}
}

void Editor::render() {
	Play_State::render();

	if (selected) {
		m_model->set_translate(selected->get_center());
		m_model->set_scale(Vector3f(100.0f, 100.0f, 100.0f));
		m_model->render();
	}
}

void Editor::laser_collide_with_object(Map_Object *m) {
	selected = m;
}

Model * Editor::m_model = nullptr;
unsigned long Editor::m_instance_count = 0lu;