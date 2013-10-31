#include "Editor.h"

class Object_Creator : public Widget_Gamestate {
	static Editor *editor;
	static Map_Object_Factory factory;

public:
	Object_Creator(Editor *e)
		: Widget_Gamestate(std::make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)), true)
	{
		editor = e;

		m_widgets.lend_Widget(create_type);
	}

	class Type_Box : public Text_Box {
	public:
		Type_Box()
			: Text_Box(Point2f(200.0f, 100.0f), Point2f(620.0f, 100.0f), "system_36_800x600", "", get_Colors()["white"], true)
		{
		}
		
		//TODO clear text on load

		void on_accept() {
			Map_Object *m = factory.make(get_text(), editor->get_player_position());
			if (m)
				editor->insert_object(m);

			get_Game().pop_state();
		}

	} create_type;
};

Editor *Object_Creator::editor;
Map_Object_Factory Object_Creator::factory;

Editor::Editor(const std::string &map_name_)
: Play_State(map_name_),
parameter(param_translate),
selected(nullptr)
{
	if (!m_instance_count) {
		m_model = new Model("models/crate.3ds");
	}
	++m_instance_count;

	m_menu = new Editor_Menu_State(selected);
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
		parameter = param_translate;
		break;
	case E_X:
		parameter = param_scale;
		break;
	case E_C:
		parameter = param_rotation;
		break;
	case E_N:
		get_Game().push_state(m_creator);
		break;
	case E_RIGHT:
		selected = m_map.get_next(selected);
		break;
	case E_LEFT:
		selected = m_map.get_prev(selected);
		break;

	case E_EXIT:
		get_Game().push_state(m_menu);
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
		if (parameter == param_rotation)
			adjust_rotation(-Global::pi / 4.0f, Vector3f());
		else
			adjust(Vector3f(0.0f, 0.0f, -1.0f));
	}
	if (m_controls.get_key(E_E)) {
		if (parameter == param_rotation)
			adjust_rotation(Global::pi / 4.0f, Vector3f());
		else
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
		adjust_rotation(0.0f, delta / 10.0f);
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

void Editor::adjust_rotation(const float &angle, const Vector3f &ray) {
	if (selected) {
		auto rotation = selected->get_rotation().get_rotation();
		selected->set_rotate(rotation.second + angle, rotation.first + ray);
	}
}

void Editor::render() {
	if (selected) {
		m_model->set_translate(selected->get_center());
		m_model->set_scale(Vector3f(10.0f, 10.0f, 10.0f));
	}

	m_model->render();

	Play_State::render();
}

void Editor::laser_collide_with_object(Map_Object *m) {
	selected = m;
}

Model * Editor::m_model = nullptr;
unsigned long Editor::m_instance_count = 0lu;

Map_Object *Editor_Menu_State::selected = nullptr;