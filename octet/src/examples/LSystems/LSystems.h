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

    std::string load_file(const char* file_name) {
      std::ifstream is(file_name);
      if (is.bad() || !is.is_open()) return nullptr;
      char buffer[2048];
      // loop over lines
      std::string out;
      while (!is.eof()) {
        is.getline(buffer, sizeof(buffer));
        out += buffer;
        out += "\n";
      }
      //printf("%s", out.c_str());
      return out;
    }

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
      fractal_code.clear_rules();
      turtle.clear();
      // Set branches
      // Set Leaves
      
      // Set Axiom
      fractal_code.set_axiom(settings[2].c_str());
      
      // Set rules
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
      float angle = std::stof(settings[4].c_str());
      float pp_angle = std::stof(settings[5].c_str());
      turtle.set_control_angles(angle, pp_angle);

      // Set origin
      char *origin_str = const_cast<char*>(settings[6].c_str());
      char *origin = strtok(origin_str, ",");
      while (origin != NULL) {
        //printf("File rule: %c -> ", *rules);
        float x = std::stof(origin);
        origin = strtok(NULL, ",");
        if (origin == NULL) break;
        float y = std::stof(origin);
        origin = strtok(NULL, ",");
        turtle.set_origin(x, y);
      }
    }

    void input() {
      if (is_key_going_up(key_1)) {
        load_fractal_file("fractals/fractal-tree-a.frac");
      }
      if (is_key_going_up(key_2)) {
        load_fractal_file("fractals/fractal-tree-b.frac");
      }
      if (is_key_going_up(key_3)) {
        load_fractal_file("fractals/fractal-tree-c.frac");
      }
      if (is_key_going_up(key_4)) {
        load_fractal_file("fractals/fractal-tree-d.frac");
      }
      if (is_key_going_up(key_5)) {
        load_fractal_file("fractals/fractal-tree-e.frac");
      }
      if (is_key_going_up(key_6)) {
        load_fractal_file("fractals/fractal-tree-f.frac");
      }
      if (is_key_going_up(key_7)) {
        load_fractal_file("fractals/sierpinski-arrowhead.frac");
      }
      if (is_key_going_up(key_8)) {
        load_fractal_file("fractals/sierpinski-triangle.frac");
      }
      if (is_key_going_up(key_9)) {
        load_fractal_file("fractals/dragon-curve.frac");
      }
      if (is_key_going_up(key_0)) {
        load_fractal_file("fractals/pythagoras-tree.frac");
      }
      if (is_key_going_up(' ')) {
        fractal_code.do_step();
      }
      if (is_key_going_up(key_r)) {
        turtle.clear();
      }
      if (is_key_going_up(key_left) && !is_key_down(key_shift) && !is_key_down(key_ctrl)) {
        float x, y;
        turtle.get_origin(x, y);
        x -= 0.1f;
        turtle.set_origin(x, y);
        turtle.force_generate();
      }
      if (is_key_going_up(key_right) && !is_key_down(key_shift) && !is_key_down(key_ctrl)) {
        float x, y;
        turtle.get_origin(x, y);
        x += 0.1f;
        turtle.set_origin(x, y);
        turtle.force_generate();
      }
      if (is_key_going_up(key_up)) {
        float x, y;
        turtle.get_origin(x, y);
        y += 0.1f;
        turtle.set_origin(x, y);
        turtle.force_generate();
      }
      if (is_key_going_up(key_down)) {
        float x, y;
        turtle.get_origin(x, y);
        y -= 0.1f;
        turtle.set_origin(x, y);
        turtle.force_generate();
      }

      if (is_key_going_up(key_left) && is_key_down(key_shift)) {
        float a, pp_a;
        turtle.get_control_angles(a, pp_a);
        a = a - 1;
        turtle.set_control_angles(a, pp_a);
        turtle.force_generate();
      }
      if (is_key_going_up(key_right) && is_key_down(key_shift)) {
        float a, pp_a;
        turtle.get_control_angles(a, pp_a);
        a = a + 1;
        turtle.set_control_angles(a, pp_a);
        turtle.force_generate();
      }

      // steps the angle amount 
      if (is_key_going_up(key_left) && is_key_down(key_ctrl)) {
        float a, pp_a;
        turtle.get_control_angles(a, pp_a);
        pp_a = pp_a - 1;
        turtle.set_control_angles(a, pp_a);
        turtle.force_generate();
      }
      if (is_key_going_up(key_right) && is_key_down(key_ctrl)) {
        float a, pp_a;
        turtle.get_control_angles(a, pp_a);
        pp_a = pp_a + 1;
        turtle.set_control_angles(a, pp_a);
        turtle.force_generate();
      }
    }

  public:
    /// this is called when we construct the class before everything is initialised.
    LSystems(int argc, char **argv) : app(argc, argv) {
    }

    GLuint VBO;
    shader tree_shader;
    shader leaf_shader;
    /// this is called once OpenGL is initialized
    void app_init() {
      app_scene =  new visual_scene();
      app_scene->create_default_camera_and_lights();

      load_fractal_file("fractals/fractal-tree-d.frac");


      glGenBuffers(1, &VBO);


      tree_shader.init(load_file("shaders/tree.vert").c_str(), load_file("shaders/tree.frag").c_str());
      leaf_shader.init(load_file("shaders/leaf.vert").c_str(), load_file("shaders/leaf.frag").c_str());
    }

    //TEMPORARY testing vars
    bool animate = false;
    /// this is called to draw the world
    void draw_world(int x, int y, int w, int h) {
      // The following chunk is preset hotkeys to load diffirent files.
      input();

      if (is_key_going_down(key_d)) {
        if (!animate) {
          load_fractal_file("fractals/dragon-curve.frac");
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          turtle.set_control_angles(180, 0);
          turtle.set_origin(0, 0);
          animate = true;
        }
        else {
          animate = false;
        }
      }
      if (is_key_going_down(key_s)) {
        if (!animate) {
          load_fractal_file("fractals/sierpinski-arrowhead.frac");
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          turtle.set_control_angles(180, 0);
          turtle.set_origin(0, 0);
          animate = true;
        }
        else {
          animate = false;
        }
      }
      if (is_key_going_down(key_w)) {
        if (!animate) {
          load_fractal_file("fractals/fractal-tree-c.frac");
          fractal_code.do_step();
          fractal_code.do_step();
          fractal_code.do_step();
          turtle.set_control_angles(23, 0);
          turtle.set_origin(0, 0);
          animate = true;
        }
        else {
          animate = false;
        }
      }



      if (is_key_going_down(key_a)) {
        if (!animate) {
          animate = true;
        }
        else {
          animate = false;
        }
      }
      if (animate) {
        float a, pp_a;
        turtle.get_control_angles(a, pp_a);
        a = a - 0.5;
        turtle.set_control_angles(a, pp_a);
        turtle.force_generate();
      }

      // ---------- Generate new mesh if requested or string changed
      turtle.render(fractal_code.get_string());

      // ---------- Render
      glClearColor(0,0,0,1);
      glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

      // Branches
      std::vector<GLfloat> branches_vertices_v = turtle.get_branches_mesh();
      if (branches_vertices_v.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, branches_vertices_v.size() * sizeof(GLfloat), &branches_vertices_v[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(attribute_pos);
        glUseProgram(tree_shader.get_program());
        glDrawArrays(GL_LINES, 0, branches_vertices_v.size()/2);
        glBindVertexArray(attribute_pos);
      }

      // Leaves
      std::vector<GLfloat> leaves_vertices_v = turtle.get_leaves_mesh();
      if (leaves_vertices_v.size() > 0) {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, leaves_vertices_v.size() * sizeof(GLfloat), &leaves_vertices_v[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(attribute_pos);
        glUseProgram(leaf_shader.get_program());
        glDrawArrays(GL_TRIANGLES, 0, leaves_vertices_v.size() / 2);
        glBindVertexArray(attribute_pos);
      }
    }
  };
}
