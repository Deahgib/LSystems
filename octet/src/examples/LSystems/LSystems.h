////////////////////////////////////////////////////////////////////////////////
//
// (C) Andy Thomason 2012-2014
//
// Modular Framework for OpenGLES2 rendering on multiple platforms.
//

#include "Turtle.h"
#include "TreeString.h"

namespace octet {
  /// Scene containing a box with octet.
  class LSystems : public app {
    // scene for drawing box
    ref<visual_scene> app_scene;
    Turtle turtle;

    TreeString str;
  public:
    /// this is called when we construct the class before everything is initialised.
    LSystems(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();

      str.set_axiom("A");
      turtle.render(str.get_string());
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      if (is_key_going_down(' ')) {
        str.do_step();
      }

      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      turtle.render(str.get_string());
    }
  };
}
