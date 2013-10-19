#include <zenilib.h>

using namespace Zeni;

class Play_State : public Gamestate_Base {
private:
	Camera m_camera;
	Zeni::Time m_current_time;
	Chronometer<Time> m_chrono;
	float m_time_passed;
	float m_max_time_step;  //< Optional
	float m_max_time_steps; //< Optional

	Play_State(const Play_State &);
	Play_State operator=(const Play_State &);

	struct Controls {
		Controls() : left(false), right(false), up(false), down(false),
			roll_left(false), roll_right(false), pitch(0.0f), yaw(0.0f) {}

		bool left;
		bool right;
		bool up;
		bool down;
		bool roll_left;
		bool roll_right;

		float pitch;
		float yaw;
	} m_controls; //end struct Controls


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
	void move_camera(const float &time_step);

	void render_plane(const Point3f &top_left, const Point3f &bottom_right, const Color &c);
};