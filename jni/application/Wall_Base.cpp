#include "Wall_Base.h"

using namespace Zeni::Collision;

Wall_Base::Wall_Base(const Point3f &corner_,
	const Vector3f &scale_,
	const Quaternion &rotation_,
	const String &model_file_)
	: m_corner(corner_),
	m_scale(scale_),
	m_rotation(rotation_)
{
	m_instance = &m_model_map[model_file_];
	m_model = m_instance->model;

	if (!m_instance->count)
		m_model = new Model(model_file_);
	++m_instance->count;

	m_size = m_model->get_extents().upper_bound - m_model->get_extents().lower_bound;
	m_size.multiply_by(m_scale);

	create_body();
}

Wall_Base::Wall_Base(const Wall_Base &rhs)
: m_corner(rhs.m_corner),
m_scale(rhs.m_scale),
m_rotation(rhs.m_rotation),
m_instance(rhs.m_instance),
m_model(rhs.m_model)
{
	++m_instance->count;

	create_body();
}

Wall_Base & Wall_Base::operator=(const Wall_Base &rhs) {
	m_corner = rhs.m_corner;
	m_scale = rhs.m_scale;
	m_rotation = rhs.m_rotation;

	create_body();

	return *this;
}

Wall_Base::~Wall_Base() {
	if (!--m_instance->count) {
		delete m_model;
		m_model = nullptr;
	}
}

void Wall_Base::render() const {
	const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();

	m_model->set_translate(m_corner);
	m_model->set_scale(m_scale);
	m_model->set_rotate(rotation.second, rotation.first);

	m_model->render();
}

bool Wall_Base::intersects(const Parallelepiped &p) const {
	return m_body.intersects(p);
}

void Wall_Base::collide() {

}

void Wall_Base::create_body() {
	m_body = Parallelepiped(m_corner,
		m_rotation * m_size.get_i(),
		m_rotation * m_size.get_j(),
		m_rotation * m_size.get_k());
}

std::map<String, Model_Instance> Wall_Base::m_model_map;
