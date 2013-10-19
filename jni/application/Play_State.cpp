#include "Play_State.h"

Play_State::Play_State()
: m_camera(Point3f(0.0f, 0.0f, 0.0f)),
m_time_passed(0.0f),
m_max_time_step(1.0f / 20.0f), // make the largest physics step 1/20 of a second
m_max_time_steps(10.0f) // allow no more than 10 physics steps per frame
{
	set_pausable(true);
}

void Play_State::on_push() {
	get_Window().set_mouse_state(Window::MOUSE_RELATIVE);

	m_chrono.start();
}

void Play_State::on_pop() {
	m_chrono.stop();

	get_Controllers().reset_vibration_all();
}

void Play_State::on_cover() {
	get_Controllers().reset_vibration_all();
}

void Play_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
	m_controls.pitch += event.yrel;
	m_controls.yaw -= event.xrel;
}

void Play_State::on_key(const SDL_KeyboardEvent &event) {
	switch (event.keysym.sym) {
	case SDLK_LEFT:
	case SDLK_a:
		m_controls.left = event.type == SDL_KEYDOWN;
		break;

	case SDLK_RIGHT:
	case SDLK_d:
		m_controls.right = event.type == SDL_KEYDOWN;
		break;

	case SDLK_UP:
	case SDLK_w:
		m_controls.up = event.type == SDL_KEYDOWN;
		break;

	case SDLK_DOWN:
	case SDLK_s:
		m_controls.down = event.type == SDL_KEYDOWN;
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
	// Update time_passed
	const float time_passed = m_chrono.seconds();
	float time_step = time_passed - m_time_passed;
	m_time_passed = time_passed;

	/* Shrink time passed to an upper bound
	*
	* If your program is ever paused by your computer for 10
	* minutes and then allowed to continue running, you don't want
	* it to pause at the physics loop for another 10 minutes.
	*/
	if (time_step / m_max_time_step > m_max_time_steps)
		time_step = m_max_time_steps * m_max_time_step;

	while (time_step > m_max_time_step) {
		move_camera(time_step);

		time_step -= m_max_time_step;
	}

	move_camera(time_step);
	time_step = 0.0f;

}

void Play_State::move_camera(const float &time_step) {
	m_camera.move_left_xy((m_controls.left - m_controls.right) * time_step * 100.0f);
	m_camera.move_forward_xy((m_controls.up - m_controls.down) * time_step * 100.0f);

	m_camera.adjust_pitch(m_controls.pitch * time_step);
	m_camera.adjust_yaw(m_controls.yaw * time_step);
	m_camera.adjust_roll((m_controls.roll_right - m_controls.roll_left) * time_step);

	m_controls.pitch = 0;
	m_controls.yaw = 0;
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
	vr.set_3d(m_camera);

	render_plane(Point3f(100.0f, 100.0f, -10.0f), Point3f(-100.0f, -100.0f, -10.0f), get_Colors()["purple"]);
	render_plane(Point3f(100.0f, 100.0f, -10.0f), Point3f(-100.0f, 100.0f, 100.0f), get_Colors()["green"]);
}