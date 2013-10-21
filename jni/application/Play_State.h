#pragma once

#include <zenilib.h>
#include "Player.h"
#include "Wall.h"
#include "Map.h"

using namespace Zeni;

class Play_State : public Gamestate_II {
private:
	Player m_player;
	Map m_map;

	Light m_light;

	Zeni::Time_HQ time_passed;

	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

	struct Controls {
		Controls() : forward(0.0f), left(0.0f), back(0.0f), right(0.0f),
			joy_x(0.0f), joy_y(0.0f), roll_left(false), roll_right(false) {}

		float forward;
		float left;
		float back;
		float right;
		float joy_x;
		float joy_y;
		bool roll_left;
		bool roll_right;
	} m_controls; //end struct Controls

	bool m_moved;

public:
	Play_State();

	void on_push();
	void on_pop();
	void on_cover();

	void on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action);
	void on_mouse_motion(const SDL_MouseMotionEvent &event);
	void on_key(const SDL_KeyboardEvent &event);

	void perform_logic();

	void render();

private:
	void partial_step(const float &time_step, const Vector3f &velocity);

	void render_plane(const Point3f &top_left, const Point3f &bottom_right, const Color &c);
};