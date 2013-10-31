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

enum Event_Type {
	E_EXIT = 1,
	E_LEFTRIGHT,
	E_FORWARDBACK,
	E_YAW,
	E_PITCH,
	E_BRAKE,
	E_SHOOT,
	E_ROLLLEFT,
	E_ROLLRIGHT,
	E_RESET,
	E_LOOKBACK,
	E_NOCLIP,
	E_W,
	E_A,
	E_S,
	E_D,
	E_Q,
	E_E,
	E_Z,
	E_X,
	E_C,
	E_N,
	E_RIGHT,
	E_LEFT,
	E_TAB,

	E_MAX
};

class Play_State : public Gamestate_II {
private:
	Light m_dir_light;
	Ship m_enemy;
	Lap_Time m_time;
	Finish_Line m_finish;

	std::vector<Laser *> lasers;

	Zeni::Time_HQ time_passed;

	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

	struct Controls {
		Controls() : forward(0.0f), left(0.0f), back(0.0f), right(0.0f),
			joy_x(0.0f), joy_y(0.0f), roll_left(false), roll_right(false), 
			shoot(false), brake(0.0f), boost(0.0f) {}

		float forward;
		float left;
		float back;
		float right;
		float joy_x;
		float joy_y;
		bool roll_left;
		bool roll_right;
		bool shoot;
		float brake;
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
	Player m_player;
	Map m_map;
	virtual void laser_collide_with_object(Map_Object *colliding);
	void render_3d() const;
	void render_2d() const;

public:
	Play_State(const std::string &map_name_);

	void on_push();
	void on_pop();
	void on_cover();

	void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);

	void reset();

	void perform_logic();

	void render();

private:
	void partial_ship_step(const float &time_step, const Vector3f &velocity);
	void partial_ship_pitch(const float &time_step, const float &phi);
	void partial_ship_yaw(const float &time_step, const float &theta);
	void partial_ship_roll(const float &time_step, const float &rho);

	void partial_laser_step(const float &time_step, const Vector3f &velocity);

	void render_plane(const Point3f &top_left, const Point3f &bottom_right, const Color &c);

	bool check_collisions();

	bool m_controller_mouse;
	Zeni::Window::Mouse_State m_mouse_state;
};