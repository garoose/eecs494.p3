#include "Play_State.h"

Play_State::Play_State()
	: m_player(Camera(Point3f(0.0f, 0.0f, 50.0f), Quaternion(), 1.0f, 10000.0f),
		Vector3f(100.0f, 100.0f, 100.0f), Vector3f(-20.0f, 0.0f, 0.0f))
{
	set_pausable(true);
}

void Play_State::on_push() {
	get_Window().set_mouse_state(Window::MOUSE_RELATIVE);
}

void Play_State::on_pop() {
	get_Controllers().reset_vibration_all();
}

void Play_State::on_cover() {
	get_Controllers().reset_vibration_all();
}

void Play_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
	m_player.adjust_pitch(event.yrel / 500.0f);
	m_player.turn_left_xy(-event.xrel / 500.0f);
}

void Play_State::on_key(const SDL_KeyboardEvent &event) {
	switch (event.keysym.sym) {
	case SDLK_a:
		m_controls.left = event.type == SDL_KEYDOWN;
		break;

	case SDLK_d:
		m_controls.right = event.type == SDL_KEYDOWN;
		break;

	case SDLK_w:
		m_controls.forward = event.type == SDL_KEYDOWN;
		break;

	case SDLK_s:
		m_controls.back = event.type == SDL_KEYDOWN;
		break;

	case SDLK_q:
		m_controls.roll_left = event.type == SDL_KEYDOWN;
		break;

	case SDLK_e:
		m_controls.roll_right = event.type == SDL_KEYDOWN;
		break;

	default:
		Gamestate_Base::on_key(event);
		break;
	}
}

void Play_State::on_controller_button(const SDL_ControllerButtonEvent &event) {
	if (event.button == SDL_CONTROLLER_BUTTON_BACK && event.state == SDL_PRESSED)
		get_Game().push_Popup_Menu_State();
}

void Play_State::perform_logic() {
	const Time_HQ current_time = get_Timer_HQ().get_time();
	float processing_time = float(current_time.get_seconds_since(time_passed));
	time_passed = current_time;

	/** Get forward and left vectors in the XY-plane **/
	const Vector3f forward = m_player.get_camera().get_forward().get_ij().normalized();
	const Vector3f left = m_player.get_camera().get_left().get_ij().normalized();

	/** Get velocity vector split into a number of axes **/
	const Vector3f velocity = (m_controls.forward - m_controls.back) * 50.0f * forward
		+ (m_controls.left - m_controls.right) * 50.0f * left;
	const Vector3f x_vel = velocity.get_i();
	const Vector3f y_vel = velocity.get_j();
	const Vector3f z_vel = velocity.get_k();

	/** Bookkeeping for sound effects **/
	if (velocity.magnitude() != 0.0f)
		m_moved = true;

	/** Keep delays under control (if the program hangs for some time, we don't want to lose responsiveness) **/
	if (processing_time > 0.1f)
		processing_time = 0.1f;

	/** Physics processing loop**/
	for (float time_step = 0.05f;
		processing_time > 0.0f;
		processing_time -= time_step)
	{
		if (time_step > processing_time)
			time_step = processing_time;

		/** Try to move on each axis **/
		partial_step(time_step, x_vel);
		partial_step(time_step, y_vel);
		partial_step(time_step, z_vel);
	}
}

void Play_State::move_camera(const float &time_step) {
	m_player.adjust_roll((m_controls.roll_right - m_controls.roll_left) * time_step);
}

void Play_State::render_plane(const Point3f &top_left, const Point3f &bottom_right, const Color &c) {
	Video &vr = get_Video();

	Vertex3f_Color p0(top_left, c);
	Vertex3f_Color p1(Point3f(bottom_right.x, top_left.y, top_left.z), c);
	Vertex3f_Color p2(bottom_right, c);
	Vertex3f_Color p3(Point3f(top_left.x, bottom_right.y, bottom_right.z), c);

	Quadrilateral<Vertex3f_Color> q(p0, p1, p2, p3);

	vr.render(q);
}

void Play_State::render() {
	Video &vr = get_Video();
	vr.set_3d(m_player.get_camera());

	render_plane(Point3f(100.0f, 100.0f, -10.0f), Point3f(-100.0f, -100.0f, -10.0f), get_Colors()["purple"]);
	render_plane(Point3f(100.0f, 100.0f, -10.0f), Point3f(-100.0f, 100.0f, 100.0f), get_Colors()["green"]);

	m_player.render();
}

void Play_State::partial_step(const float &time_step, const Vector3f &velocity) {
	m_player.set_velocity(velocity);
	const Point3f backup_position = m_player.get_camera().position;

	m_player.step(time_step);

	/** If collision with the crate has occurred, roll things back **/
#if 0
	if (m_crate.get_body().intersects(m_player.get_body())) {
		if (m_moved)
		{
			/** Play a sound if possible **/
			m_crate.collide();
			m_moved = false;
		}

		m_player.set_position(backup_position);

		/** Bookkeeping for jumping controls **/
		if (velocity.k < 0.0f)
			m_player.set_on_ground(true);
	}
#endif
}