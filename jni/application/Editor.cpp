#include "Editor.h"

Editor::Editor(const std::string &map_name_)
: Play_State(map_name_),
parameter(param_translate),
selected(nullptr)
{
	if (!m_instance_count) {
		m_model = new Model("models/crate.3ds");
	}
	++m_instance_count;
}

Editor::~Editor() {
	if (!--m_instance_count) {
		delete m_model;
		m_model = nullptr;
	}
}

void Editor::on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action) {
	switch (action) {
	case E_W:
		m_controls.w = confidence ? true : false;
		break;
	case E_A:
		m_controls.a = confidence ? true : false;
		break;
	case E_S:
		m_controls.s = confidence ? true : false;
		break;
	case E_D:
		m_controls.d = confidence ? true : false;
		break;
	case E_Q:
		m_controls.q = confidence ? true : false;
		break;
	case E_E:
		m_controls.e = confidence ? true : false;
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
	case E_RIGHT:
		selected = m_map.get_next(selected);
		break;
	case E_LEFT:
		selected = m_map.get_prev(selected);
		break;
	}

	Play_State::on_event(id, confidence, action);
}

void Editor::perform_logic() {
	if (m_controls.w)
		adjust(Vector3f(0.5f, 0.0f, 0.0f));
	if (m_controls.a)
		adjust(Vector3f(0.0f, 0.5f, 0.0f));
	if (m_controls.s)
		adjust(Vector3f(-0.5f, 0.0f, 0.0f));
	if (m_controls.d)
		adjust(Vector3f(0.0f, -0.5f, 0.0f));
	if (m_controls.q) {
		if (parameter == param_rotation)
			adjust_rotation(-Global::pi_over_two, Vector3f());
		else
			adjust(Vector3f(0.0f, 0.0f, -0.5f));
	}
	if (m_controls.e) {
		if (parameter == param_rotation)
			adjust_rotation(Global::pi_over_two, Vector3f());
		else
			adjust(Vector3f(0.0f, 0.0f, 0.5f));
	}

	Play_State::perform_logic();
}

void Editor::adjust(const Vector3f &delta) {
	switch (parameter) {
	case param_translate:
		adjust_translate(delta);
		break;
	case param_scale:
		adjust_scale(delta);
		break;
	case param_rotation:
		adjust_rotation(0.0f, delta);
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