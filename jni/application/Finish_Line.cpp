#include "Finish_Line.h"

#include "Ship.h"

using namespace Zeni::Collision;

Finish_Line::Finish_Line(const Point3f &corner_,
	const Vector3f &scale_,
	const Quaternion &rotation_)
	: m_source(new Sound_Source(get_Sounds()["party_horn"])),
	m_corner(corner_),
	m_scale(scale_),
	m_rotation(rotation_),
	m_crossed(false)
{
	if (!m_instance_count)
		//m_model = new Model("models/crate.3ds");
		m_model = nullptr;
	++m_instance_count;

	create_body();
}

Finish_Line::~Finish_Line() {
	delete m_source;

	if (!--m_instance_count) {
		delete m_model;
		m_model = nullptr;
	}
}

void Finish_Line::reset() {
	m_crossed = false;
}

void Finish_Line::render() const {
	const std::pair<Vector3f, float> rotation = m_rotation.get_rotation();

	Video &vr = get_Video();

	Vertex3f_Texture p0(Vector3f(m_corner), Point2f(0.0f, 0.0f));
	Vertex3f_Texture p1(Vector3f(m_corner.x + m_scale.x, m_corner.y, m_corner.z), Point2f(0.0f, 1.0f));
	Vertex3f_Texture p2(Vector3f(m_corner.x, m_corner.y, m_corner.z + m_scale.z), Point2f(1.0f, 1.0f));
	Vertex3f_Texture p3(Vector3f(m_corner.x, m_corner.y + m_scale.y, m_corner.z), Point2f(1.0f, 0.0f));
	Material material("finish_line");

	Quadrilateral<Vertex3f_Texture> quad(p0, p1, p2, p3);
	quad.fax_Material(&material);

	vr.render(quad);

	/*m_model->set_translate(m_corner);
	m_model->set_scale(m_scale);
	m_model->set_rotate(rotation.second, rotation.first);

	m_model->render();*/
}

bool Finish_Line::intersects(const Ship &s) const {
	return m_body.intersects(s.get_body());
}

void Finish_Line::collide() {
	if (!m_source->is_playing())
		m_source->play();

	m_crossed = true;
}

const Vector3f &Finish_Line::get_surface() const {
	return m_rotation * m_scale;
}

void Finish_Line::create_body() {
	m_body = Parallelepiped(m_corner,
		m_rotation * m_scale.get_i(),
		m_rotation * m_scale.get_j(),
		m_rotation * m_scale.get_k());

	m_source->set_position(m_corner + m_rotation * m_scale / 2.0f);
}

Model * Finish_Line::m_model = nullptr;
unsigned long Finish_Line::m_instance_count = 0lu;