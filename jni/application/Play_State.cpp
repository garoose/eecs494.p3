#include <string>

#include "Play_State.h"
#include "Win_State.h"
#include "Wall.h"

using std::string;

Play_State::Play_State(const string &map_name_)
	: m_map(map_name_),
	m_moved(false),
	m_noclip(false),
	m_player(Camera(Point3f(0.0f, 0.0f, 0.0f), Quaternion(), 1.0f, 10000.0f), Vector3f(1.0f, 1.0f, 1.0f))
{
	set_pausable(true);

	m_finish = m_map.get_finish();

	get_Textures().lend("SHIP_BAC.PNG", &ship_rear_sprite, true);
	ship_rear_sprite.append_frame("ship_back_off");
	ship_rear_sprite.append_frame("ship_back_on");

	m_dir_light.set_light_type(LIGHT_DIRECTIONAL);
	m_dir_light.diffuse = Color(1.0f, 0.4f, 0.4f, 0.f);
	m_dir_light.specular = Color(1.0f, 0.4f, 0.4f, 0.4f);

	/*** Joyfun ***/

	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), E_EXIT);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_BACK), E_EXIT);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_LEFTX /* left-right */), E_LEFTRIGHT);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_LEFTY /* forward-backward */), E_FORWARDBACK);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_RIGHTX /* yaw */), E_YAW);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_RIGHTY /* pitch */), E_PITCH);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_TRIGGERLEFT /* brake */), E_BRAKE);
	set_action(Zeni_Input_ID(SDL_CONTROLLERAXISMOTION, SDL_CONTROLLER_AXIS_TRIGGERRIGHT /* shoot */), E_SHOOT);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_LEFTSHOULDER /* rotate left */), E_ROLLLEFT);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER /* rotate right */), E_ROLLRIGHT);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_START /* reset */), E_RESET);
	set_action(Zeni_Input_ID(SDL_CONTROLLERBUTTONDOWN, SDL_CONTROLLER_BUTTON_B /* look back */), E_LOOKBACK);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_F1 /* noclip */), E_NOCLIP);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_w), E_W);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_a), E_A);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_s), E_S);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_d), E_D);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_q), E_Q);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_e), E_E);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_z), E_Z);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_x), E_X);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_c), E_C);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_n), E_N);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_f), E_F);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RIGHT), E_RIGHT);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LEFT), E_LEFT);
	set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_TAB), E_TAB);
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

	m_time.pause();
}

void Play_State::on_uncover() {
	m_time.unpause();
}

void Play_State::on_event(const Zeni_Input_ID &id, const float &confidence, const int &action) {
	if (confidence > 0.5f)
		get_Game().write_to_console(ulltoa(id.which) + '.' + ulltoa(id.subid) + ':' + ulltoa(action));

	switch (action) {
	case E_EXIT: // back
		if (confidence == 1.0f) {
			Game &game = get_Game();
			game.push_state(new Popup_Menu_State);
		}
		break;

	case E_LEFTRIGHT: // left joy x
		m_controls.right = m_controls.left = 0.0f;
		if (confidence > 0.1 || confidence < -0.1)
			m_controls.right = confidence;
		break;

	case E_FORWARDBACK: // lefy joy y
		if (confidence > 0.1 || confidence < -0.1)
			m_controls.forward = -confidence;
		else
			m_controls.forward = 0.0f;
		break;

	case E_YAW: // right joy x
		m_controls.joy_x = -confidence;
		break;

	case E_PITCH: // right joy y
		m_controls.joy_y = confidence;
		break;

	case E_BRAKE: // left trigger
		m_controls.brake = confidence;
		break;

	case E_SHOOT: // right trigger
		//shoot a laser
		m_controls.shoot = confidence > 0.5f ? true : false;
		break;

	case E_ROLLLEFT: // left bumper
		m_controls.roll_left = confidence == 1.0f ? true : false;
		break;

	case E_ROLLRIGHT: // right bumper
		m_controls.roll_right = confidence == 1.0f ? true : false;
		break;

	case E_RESET: // start
		if (confidence == 1.0f)  {
			reset();
		}
		break;

	case E_LOOKBACK:
		m_player.reverse_camera();
		break;

	case E_NOCLIP: // noclip
		if (confidence == 1.0f)
			m_noclip = !m_noclip;
		break;

	default:
		break;
	}
}

void Play_State::reset() {
	get_Controllers().reset_vibration_all();
	m_map.reset();
	m_player.reset();
	m_time.reset();
	m_finish->reset();
	m_finish_delay.stop();
	m_finish_delay.set(0.0f);
	for (auto it = lasers.begin(); it != lasers.end();) {
		auto laser = (*it);
		it = lasers.erase(it);
		delete laser;
	}
}

void Play_State::on_finish_cross() {
	auto time = m_time.get_time();
	reset();
	get_Game().push_state(new Win_State(time));
}

void Play_State::perform_logic() {
	const Time_HQ current_time = get_Timer_HQ().get_time();
	float processing_time = float(current_time.get_seconds_since(time_passed));
	time_passed = current_time;

	/** Check if player has crossed the finish line **/
	if (m_finish->crossed() && !m_finish_delay.seconds())
		m_finish_delay.start();

	if (m_finish_delay.seconds() > 1.5f)
		on_finish_cross();

	/** Check if player already exploded **/
	if (m_player.is_exploded())
		reset();

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
	const Vector3f forward = m_player.get_forward().normalized();
	const Vector3f left = m_player.get_left().normalized();

	/** Find change in acceleration **/
	const float &accel = m_player.get_acceleration();
	Vector3f forward_accel = (m_controls.forward - m_controls.back) * accel * forward;
	Vector3f side_accel = (m_controls.left - m_controls.right) * accel * left;

	// let the player know if acceleration was recieved
	m_player.set_moved((forward_accel + side_accel).magnitude() ? true : false);

	Vector3f velocity = m_player.get_prev_velocity() + forward_accel + side_accel;

	/** Perform braking **/
	const float &braking = m_controls.brake;
	velocity -= velocity * braking / 20.0f;

	/** Don't go over max speed **/
	float result_speed = velocity.magnitude();
	while (result_speed > m_player.get_max_speed()) {
		velocity += velocity * -0.01f;
		result_speed = velocity.magnitude();
	}

	/** Air resistance **/
	velocity += m_player.get_prev_velocity() * -0.009f;

	m_player.clear_prev_velocity();

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
		partial_ship_pitch(time_step, m_controls.joy_y * 1.6f);
		partial_ship_yaw(time_step, m_controls.joy_x * 2.2f);
		partial_ship_roll(time_step, (m_controls.roll_right - m_controls.roll_left) * 1.7f);

		partial_ship_step(time_step, x_vel);
		partial_ship_step(time_step, y_vel);
		partial_ship_step(time_step, z_vel);

		//step lasers
		for (auto it = lasers.begin(); it != lasers.end();) {
			auto laser = (*it);
			laser->step(time_step);

			if (!laser->is_exploding()) {
				//collide lasers with map
				Map_Object *colliding = m_map.intersects(laser->get_body());
				if (colliding) {
					laser->collide();
					laser_collide_with_object(colliding);
				}

				//collide lasers with ships
				if (laser->intersects(m_player.get_body())) {
					laser->collide();
					m_player.collide_with_laser();
				}
			}

			//remove any destroyed lasers
			if (laser->can_destroy()) {
				it = lasers.erase(it);
				delete laser;
			} else {
				++it;
			}
		} // end lasers

		// step the map
		m_map.step(time_step);
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

void Play_State::laser_collide_with_object(Map_Object *colliding) {
	colliding->collide_with_laser();
}

void Play_State::render() {
	render_3d();
	render_3d_stop();
	render_2d();
}

void Play_State::render_3d() {
	Video &vr = get_Video();

	// set player camera as view point
	vr.set_3d(m_player.get_camera());

	// Lighting
	vr.set_lighting(true);
	vr.set_ambient_lighting(Color(1.0f, 0.0f, 0.0f, 0.0f));
	//vr.set_ambient_lighting(Color(1.0f, 0.1f, 0.1f, 0.1f));
	//vr.set_Light(0, m_dir_light);
	vr.set_Light(1, m_player.get_headlight());
	vr.set_Light(2, m_player.get_taillight());
	
	m_map.render();

	if (m_player.moved())
		ship_rear_sprite.set_current_frame(1);
	m_player.render();
	ship_rear_sprite.set_current_frame(0);

	//render lasers
	for (auto it = lasers.begin(); it != lasers.end(); ++it) {
		(*it)->render();
	}

	// render finish line last so we can see through it
	m_finish->render();
}

void Play_State::render_3d_stop() {
	Video &vr = get_Video();
	// Restore normal lighting
	vr.set_lighting(false);
}

void Play_State::render_2d() {
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
	if (m_finish->crossed()) {
		Font &fr = get_Fonts()["title"];
		String msg = "Finish Crossed!";
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
			Point2f(float(get_Window().get_size().x / 2), 100.0f - (0.5f * fr.get_text_height())),
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

	/** Collide with the map **/
	Map_Object *colliding = m_map.intersects(m_player.get_body());
	if (!m_noclip && colliding) {
		colliding->collide();
		has_collided = true;
		bounce = -1.0f;
	}

	/** collide camera with walls **/
	if (m_map.intersects(m_player.get_camera_body())) {
		m_player.adjust_camera_offset(Vector3f(-0.4f, 0.0f, 0.0f));
	} else {
		m_player.adjust_camera_offset(Vector3f(0.2f, 0.0f, 0.0f));
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
		m_player.add_prev_velocity(velocity * bounce);
	}
	else {
		m_player.add_prev_velocity(velocity);
	}

	bool was_crossed = m_finish->crossed();

	if (m_finish->intersects(m_player.get_body())) {
		if (!was_crossed)
		{
			/** Play a sound if possible **/
			m_finish->collide();
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