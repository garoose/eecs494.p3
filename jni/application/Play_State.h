#pragma once

#include <zenilib.h>
#include <vector>
#include <string>

#include "Player.h"
#include "Wall.h"
#include "Map.h"
#include "Lap_Time.h"
#include "Finish_Line.h"
#include "Laser.h"

using namespace Zeni;

class Play_State : public Gamestate_II {
private:
	Player m_player;
	Map m_map;
	Lap_Time m_time;
	Finish_Line m_finish;

	Vector3f prev_ship_velocity;

	std::vector<Laser *> lasers;

	Zeni::Time_HQ time_passed;

	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

	struct Controls {
		Controls() : forward(0.0f), left(0.0f), back(0.0f), right(0.0f),
			joy_x(0.0f), joy_y(0.0f), roll_left(false), roll_right(false), shoot(false), boost(0.0f) {}

		float forward;
		float left;
		float back;
		float right;
		float joy_x;
		float joy_y;
		bool roll_left;
		bool roll_right;
		bool shoot;
		float boost;
	} m_controls; //end struct Controls

	struct Bump {
		float steps;
		bool m_vibrate;

		Bump() : steps(0.0f), m_vibrate(false) {}

		void bump() {
			steps = 0.0f;
			m_vibrate = true;
		}

		void vibrate() {
			if (m_vibrate) {
				steps++;
				get_Controllers().set_vibration(0, 0.7f, 0.7f);
			}

			if (m_vibrate && steps > 8.0f) {
				get_Controllers().set_vibration(0, 0.0f, 0.0f);
				m_vibrate = false;
			}
		}
	} m_bump;

	bool m_moved;
	bool m_noclip;

protected:
	virtual void laser_collide_with_wall(const Map_Object *) {}

	void render_3d() const;
	void render_2d() const;

public:
	Play_State(const std::string &map_name_);

	void on_push();
	void on_pop();
	void on_cover();

	void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);
	void on_mouse_motion(const SDL_MouseMotionEvent &event);
	void on_key(const SDL_KeyboardEvent &event);

	void reset();

	void perform_logic();

	void render();

private:
	void partial_ship_step(const float &time_step, const Vector3f &velocity);
	void partial_laser_step(const float &time_step, const Vector3f &velocity);

	void render_plane(const Point3f &top_left, const Point3f &bottom_right, const Color &c);

	bool check_collisions();

	bool m_controller_mouse;
	Zeni::Window::Mouse_State m_mouse_state;
};