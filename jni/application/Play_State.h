#include <zenilib.h>

using namespace Zeni;

#include "Player.h"

class Play_State : public Gamestate_Base {
private:
	Player m_player;
	Zeni::Time m_current_time;

	Zeni::Time_HQ time_passed;

	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

	struct Controls {
		Controls() : forward(false), left(false), back(false), right(false),
			roll_left(false), roll_right(false) {}

		bool forward;
		bool left;
		bool back;
		bool right;
		bool roll_left;
		bool roll_right;
	} m_controls; //end struct Controls

	bool m_moved;

public:
	Play_State();

	void on_push();
	void on_pop();
	void on_cover();

	void on_mouse_motion(const SDL_MouseMotionEvent &event);
	void on_key(const SDL_KeyboardEvent &event);
	void on_controller_button(const SDL_ControllerButtonEvent &event);

	void perform_logic();

	void render();

private:
	void partial_step(const float &time_step, const Vector3f &velocity);

	void move_camera(const float &time_step);

	void render_plane(const Point3f &top_left, const Point3f &bottom_right, const Color &c);
};