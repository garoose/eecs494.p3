/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>

#include "Play_State.h"
#include "Level_Select_State.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
	  render_image("stars", Point2f(), Point2f(800, 600));

	  Widget_Gamestate::render();

	  render_image("instructions", Point2f(), Point2f(800, 600));
  }
};

typedef Title_State<Level_Select_State, Instructions_State> Title_State_T;

class Title_State_Custom : public Title_State_T {
public:
	Title_State_Custom()
		: Title_State_T("")
	{
			m_widgets.unlend_Widget(title);
			play_button.text = "Level Select";
			//play_button.lend_Renderer(new Widget_Renderer_Color(get_Colors()["orange"]));
			//play_button.give_Renderer(new Widget_Renderer_Tricolor(get_Colors()["orange"], get_Colors()["orange"], get_Colors()["yellow"], get_Colors()["white"],get_Colors()["white"],get_Colors()["white"]));
	}

	void render() {
		render_image("stars", Point2f(), Point2f(800, 600));

		Title_State_T::render();

		render_image("logo", Point2f(150.0f, 25.0f), Point2f(650.0f, 225.0f));
	}
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("Space Race 7000");

	  get_Controllers();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();

      return new Title_State_Custom;
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}