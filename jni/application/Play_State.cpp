#include <string>

#include "Play_State.h"
#include "Wall.h"

using std::string;

Play_State::Play_State(const string &map_name_)
	: m_map(map_name_),
	m_moved(false),
	m_noclip(false),
	m_player(Camera(Point3f(-100.0f, 0.0f, 50.0f), Quaternion(), 1.0f, 10000.0f), Vector3f(1.0f, 1.0f, 1.0f)),
	m_enemy(Point3f(10.0f, 100.0f, 50.0f), Vector3f(1.0f, 1.0f, 1.0f), 100.0f, 1.0f),
	m_finish(Point3f(560.0f, 792.0f, -365.0f), Vector3f(-60.0f, -5.0f, -60.0f))
{
	set_pausable(true);

	prev_ship_velocity = Vector3f();

	/*** Joyfun ***/

	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), 1);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_BACK), 1);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_LEFTX /* left-right */), 2);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_LEFTY /* forward-backward */), 3);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_RIGHTX /* yaw */), 4);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_RIGHTY /* pitch */), 5);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_TRIGGERLEFT /*  */), 6);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_TRIGGERRIGHT /*  */), 7);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_LEFTSHOULDER /* rotate left */), 8);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER /* rotate right */), 9);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_START /* reset */), 10);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_F1), 11 /* noclip */);
}

void Play_State::on_push() {
	m_controller_mouse = get_Game().controller_mouse.enabled;
	get_Game().controller_mouse.enabled = false;

	m_mouse_state = get_Window().get_mouse_state();
	get_Window().set_mouse_state(Window::MOUSE_RELATIVE);
}

void Play_State::on_pop() {
	get_Controllers().reset_vibration_all();

	get_Game().controller_mouse.enabled = m_controller_mouse;
	get_Window().set_mouse_state(m_mouse_state);
}

void Play_State::on_cover() {
	get_Controllers().reset_vibration_all();
}

void Play_State::on_event(const Zeni_Input_ID &id, const float &confidence, const int &action) {
	if (confidence > 0.5f)
		get_Game().write_to_console(ulltoa(id.which) + '.' + ulltoa(id.subid) + ':' + ulltoa(action));

	switch (action) {
	case 1: // back
		if (confidence == 1.0f) {
			Game &game = get_Game();
			game.push_state(new Popup_Menu_State);
		}
		break;

	case 2: // left joy x
		m_controls.right = m_controls.left = 0.0f;
		m_controls.right = confidence;
		break;

	case 3: // lefy joy y
		m_controls.forward = -confidence;
		break;

	case 4: // right joy x
		m_controls.joy_x = -confidence;
		break;

	case 5: // right joy y
		m_controls.joy_y = confidence;
		break;

	case 6: // left trigger
		break;

	case 7: // right trigger
		//shoot a laser
		m_controls.shoot = confidence > 0.5f ? true : false;
		break;

	case 8: // left bumper
		m_controls.roll_left = confidence == 1.0f ? true : false;
		break;

	case 9: // right bumper
		m_controls.roll_right = confidence == 1.0f ? true : false;
		break;

	case 10: // start
		if (confidence == 1.0f)  {
			reset();
		}
		break;
	case 11: // noclip
		if (confidence == 1.0f)
			m_noclip = !m_noclip;
		break;

	case 0:
		break;

	default:
		break;
	}
}

void Play_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
	m_player.adjust_pitch(event.yrel / 500.0f);
	m_player.adjust_yaw(-event.xrel / 500.0f);
}

void Play_State::on_key(const SDL_KeyboardEvent &event) {
	switch (event.keysym.sym) {
	case SDLK_a:
		m_controls.left = event.type == SDL_KEYDOWN ? 1.0f : 0.0f;
		break;

	case SDLK_d:
		m_controls.right = event.type == SDL_KEYDOWN ? 1.0f : 0.0f;
		break;

	case SDLK_w:
		m_controls.forward = event.type == SDL_KEYDOWN ? 1.0f : 0.0f;
		break;

	case SDLK_s:
		m_controls.back = event.type == SDL_KEYDOWN ? 1.0f : 0.0f;
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

void Play_State::reset() {
	m_player.reset();
	m_enemy.reset();
	m_time.reset();
	m_finish.reset();
	prev_ship_velocity = Vector3f();
	for (auto it = lasers.begin(); it != lasers.end();) {
		auto laser = (*it);
		lasers.erase(it);
		delete laser;
	}
}

void Play_State::perform_logic() {
	const Time_HQ current_time = get_Timer_HQ().get_time();
	float processing_time = float(current_time.get_seconds_since(time_passed));
	time_passed = current_time;

	/** Check if player already exploded **/
	if (m_player.is_exploded()) {
		reset();
	}

	/** Take care of controller vibration **/
	m_bump.vibrate();

	/** Shoot Lasers **/
	if (m_controls.shoot) {
		Laser *l = m_player.fire_laser();
		if (l) {
			lasers.push_back(m_player.fire_laser());
			m_bump.bump();
		}
	}

	/** Get forward and left vectors in the XYZ-plane **/
	const Vector3f forward = m_player.get_camera().get_forward().normalized();
	const Vector3f left = m_player.get_camera().get_left().normalized();

	/** Find change in acceleration **/
	const float &accel = m_player.get_acceleration();
	Vector3f forward_accel = (m_controls.forward - m_controls.back) * accel * forward;
	Vector3f side_accel = (m_controls.left - m_controls.right) * accel * left;

	Vector3f velocity = prev_ship_velocity + forward_accel + side_accel;

	/** Don't go over max speed **/
	float result_speed = velocity.magnitude2();
	while (result_speed > m_player.get_max_speed()) {
		velocity += velocity * -0.01f;
		result_speed = velocity.magnitude2();
	}

	/** Air resistance **/
	velocity += prev_ship_velocity * -0.01f;

	prev_ship_velocity = Vector3f();

	/** Get velocity vector split into a number of axes **/
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
		partial_ship_step(time_step, x_vel);
		partial_ship_step(time_step, y_vel);
		partial_ship_step(time_step, z_vel);

		partial_ship_pitch(time_step, m_controls.joy_y);
		partial_ship_yaw(time_step, m_controls.joy_x);
		partial_ship_roll(time_step, (m_controls.roll_right - m_controls.roll_left));

		//move enemy ships
		m_enemy.step(time_step);

		//step lasers
		for (auto it = lasers.begin(); it != lasers.end();) {
			auto laser = (*it);
			laser->step(time_step);

			//collide lasers with map
			Map_Object *colliding;
			if ((colliding = m_map.intersects(laser->get_body()))) {
				laser->collide();
				laser_collide_with_wall(colliding);
			}

			//collide lasers with ships
			if (laser->intersects(m_player.get_body())) {
				laser->collide();
				m_player.collide_with_laser();
			}
			if (laser->intersects(m_enemy.get_body())) {
				laser->collide();
				m_enemy.collide_with_laser();
			}

			//remove any destroyed lasers
			if (laser->can_destroy()) {
				it = lasers.erase(it);
				delete laser;
			} else {
				++it;
			}
		}
	} 
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
	render_3d();
	render_2d();
}

void Play_State::render_3d() const {
	Video &vr = get_Video();

	// set player camera as view point
	vr.set_3d(m_player.get_camera());

	// Lighting
	vr.set_lighting(true);
	//r.set_ambient_lighting(Color(1.0f, 0.0f, 0.0f, 0.0f));
	vr.set_ambient_lighting(Color(1.0f, 0.1f, 0.1f, 0.1f));
	vr.set_Light(0, m_player.get_headlight());

	m_map.render();
	m_finish.render();
	m_player.render();
	m_enemy.render();

	//render lasers
	for (auto it = lasers.begin(); it != lasers.end(); ++it) {
		(*it)->render();
	}

	// Restore normal lighting
	vr.set_lighting(false);
}

void Play_State::render_2d() const {
	Video &vr = get_Video();

	// Render HUD
	Point2f window_size(get_Window().get_size());
	Point2f window_center(window_size.x / 2.0f, window_size.y / 2.0f);
	vr.set_2d(std::pair<Point2f, Point2f>(Point2f(), window_size));
	vr.clear_depth_buffer();

	//render lap time
	m_time.render();

	// render crosshairs
	Vector2f crosshair_size(64.0f, 64.0f);
	render_image("crosshair", window_center - crosshair_size, window_center + crosshair_size);

	// render win message if finished
	if (m_finish.crossed()) {
		Font &fr = get_Fonts()["title"];
		String msg = "You win";
		fr.render_text(
			msg,
			Point2f(get_Window().get_size().x / 2 - fr.get_text_width(msg), 100.0f - 0.5f * fr.get_text_height()),
			get_Colors()["title_text"],
			ZENI_CENTER);
	}

	// render player health bar
	m_player.render_hp();

	// render a message if noclip is enabled
	if (m_noclip) {
		Font &fr = get_Fonts()["title"];
		String msg = "noclip";
		fr.render_text(
			msg,
			Point2f(get_Window().get_size().x / 2, 100.0f - 0.5f * fr.get_text_height()),
			get_Colors()["title_text"],
			ZENI_CENTER);
	}
}

void Play_State::partial_ship_step(const float &time_step, const Vector3f &velocity) {
	const Point3f backup_position = m_player.get_position();
	bool has_collided = false;
	float bounce = 0.0f;

	m_player.set_velocity(velocity);
	m_player.step(time_step);

	/** If collision with the map has occurred, roll things back **/
	Map_Object *colliding = m_map.intersects(m_player.get_body());
	if (!m_noclip && colliding) {
		colliding->collide();
		has_collided = true;
		bounce = -1.2f;
	}

	//collide with other ships
	if (!m_noclip && m_enemy.intersects(m_player)) {
		has_collided = true;
		bounce = -1.0f;
	}
	
	/** If collision has occurred, play a sound and roll things back **/
	if (has_collided) {
		if (m_moved) {
			m_player.collide();
			m_bump.bump();
			m_moved = false;
		}

		m_player.set_position(backup_position);
		// figure out bounce vector
		prev_ship_velocity += (velocity * bounce);
	}
	else {
		prev_ship_velocity += velocity;
	}

	bool was_crossed = m_finish.crossed();

	if (m_finish.intersects(m_player.get_body())) {
		if (!was_crossed)
		{
			/** Play a sound if possible **/
			m_finish.collide();
		}
		m_time.pause();
	}
}

void Play_State::partial_ship_pitch(const float &time_step, const float &phi) {
	bool has_collided = false;
	float bounce = 0.0f;

	m_player.adjust_pitch(time_step * phi);
		
	/** If collision with the map has occurred, roll things back **/
	Map_Object *colliding = m_map.intersects(m_player.get_body());
	if (!m_noclip && colliding) {
		colliding->collide();
		has_collided = true;
		bounce = -1.2f;
	}

	//collide with other ships
	if (!m_noclip && m_enemy.intersects(m_player)) {
		has_collided = true;
		bounce = -1.0f;
	}

	/** If collision has occurred, play a sound and roll things back **/
	if (has_collided) {
		if (m_moved) {
			m_player.collide();
			m_bump.bump();
			m_moved = false;
		}

		m_player.adjust_pitch(-time_step * phi);
		// figure out bounce vector
	}
}

void Play_State::partial_ship_yaw(const float &time_step, const float &theta) {
	bool has_collided = false;
	float bounce = 0.0f;

	m_player.adjust_yaw(time_step * theta);

	/** If collision with the map has occurred, roll things back **/
	Map_Object *colliding = m_map.intersects(m_player.get_body());
	if (!m_noclip && colliding) {
		colliding->collide();
		has_collided = true;
		bounce = -1.2f;
	}

	//collide with other ships
	if (!m_noclip && m_enemy.intersects(m_player)) {
		has_collided = true;
		bounce = -1.0f;
	}

	/** If collision has occurred, play a sound and roll things back **/
	if (has_collided) {
		if (m_moved) {
			m_player.collide();
			m_bump.bump();
			m_moved = false;
		}

		m_player.adjust_yaw(-time_step * theta);
		// figure out bounce vector
	}
}

void Play_State::partial_ship_roll(const float &time_step, const float &rho) {
	bool has_collided = false;
	float bounce = 0.0f;

	m_player.adjust_roll(time_step * rho);

	/** If collision with the map has occurred, roll things back **/
	Map_Object *colliding = m_map.intersects(m_player.get_body());
	if (!m_noclip && colliding) {
		colliding->collide();
		has_collided = true;
		bounce = -1.2f;
	}

	//collide with other ships
	if (!m_noclip && m_enemy.intersects(m_player)) {
		has_collided = true;
		bounce = -1.0f;
	}

	/** If collision has occurred, play a sound and roll things back **/
	if (has_collided) {
		if (m_moved) {
			m_player.collide();
			m_bump.bump();
			m_moved = false;
		}

		m_player.adjust_roll(-time_step * rho);
		// figure out bounce vector
	}
}