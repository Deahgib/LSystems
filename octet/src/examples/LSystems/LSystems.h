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

    TreeString fractal_code;

    void load_fractal_file(const char* file_name) {
      std::vector<std::string> settings;

      std::ifstream is(file_name);
      if (is.bad() || !is.is_open()) return;
      // store the line here
      char buffer[2048];
      // loop over lines
      while (!is.eof()) {
        is.getline(buffer, sizeof(buffer));
        // loop over columns
        char *b = buffer;
        //printf("%c\n", *b);
        if (*b == '#') continue; // Ignore lines starting with #

        for (int col = 0; ; ++col) {
          char *e = b;
          while (*e != 0 && *e != ':') ++e;

          // now b -> e contains the chars in a column
          if (col == 1) {
            std::string sett = b;
            settings.push_back(sett);
          }

          if (*e != ':') break;
          b = e + 1;
        }
      }
      /*
      for (int i = 0; i < settings.size(); ++i) {
        printf("File setting %i: %s\n", (i+1) ,settings[i].c_str());
      }*/


      // Clear previous settings.
      // Set branches
      // Set Leaves
      // Set Axiom
      fractal_code.set_axiom(settings[2].c_str());
      // Set rules
      fractal_code.clear_rules();
      char *rule_str = const_cast<char*>(settings[3].c_str());
      char *rules = strtok(rule_str, ">,");
      while (rules != NULL) {
        //printf("File rule: %c -> ", *rules);
        char control = *rules;
        rules = strtok(NULL, ">,");
        if (rules == NULL) break; // Error reading the rules line.
        //printf("%s\n", rules);
        std::string conv = rules;
        rules = strtok(NULL, ">,");

        fractal_code.insert_rule(control, conv);
      }
      // Set angles
      
    }

  public:
    /// this is called when we construct the class before everything is initialised.
    LSystems(int argc, char **argv) : app(argc, argv) {
    }

    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();

      load_fractal_file("tree_template.txt");

      //fractal_code.set_axiom("0");
      //fractal_code.clear_rules();
      //fractal_code.insert_rule('0', "1[0]0");
      //fractal_code.insert_rule('1', "11");
    }

    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      if (is_key_going_down(' ')) {
        fractal_code.do_step();
      }

      // ---------- Render
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
      turtle.render(fractal_code.get_string());
    }
  };
}
