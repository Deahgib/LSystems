
namespace octet {
  class Turtle {
  private:

    struct point {
      float x;
      float y;
    };

    struct bearing {
      point pos;
      float angle;
    };

    struct line {
      point start;
      point end;
      float r, g, b;
    };

    // triangle is drawn clockwise, make sure p1->p2->p3 are in clockwise order.
    struct triangle {
      point p1;
      point p2;
      point p3;
    };

    // Turtle variables
    float facing_angle;
    float unit_length;

    // Control varaibles set from file
    float angle;
    float pp_angle; // Push pop angle

    // Used to render only when the input string changes
    unsigned int code_length;

    std::vector<GLfloat> branches_mesh;
    std::vector<GLfloat> leaves_mesh;
    std::vector<point> all_points;
    std::vector<line> all_lines;
    std::vector<triangle> all_leaves;

    // Used to push and pop the turtle back to positions when drawing a structure with many loose ends like a tree
    std::vector<bearing> point_stack;
    point origin;
    point last_point;

    float get_radians(float deg) {
      return (deg * 3.14159265359f) / 180;
    }

    point get_next_projected_point() {
      // Translate negative values into positive counterparts eg -60' === +300'
      if (facing_angle < 0) {
        while (facing_angle < 0) {
          facing_angle = 360 + facing_angle;
        }
      }
      int quadrant = ( ( (int)facing_angle / 90) % 4) + 1;
      /* OLD WAY FMOD is faster than cast juggling.
      float float_remainder = facing_angle - (float)((int)facing_angle);
      printf("%f\n", float_remainder);
      float remainder_angle = (float)((int)facing_angle % 90) + float_remainder;
      */
      float remainder_angle = fmod(facing_angle, 90);
      remainder_angle = get_radians(remainder_angle);
      //printf("\nAngle: %f\n", facing_angle);
      //printf("Quadrant: %i\n", quadrant);
      //printf("Internal Angle: %i\n", remainder_angle);

      float dx, dy;
      switch (quadrant) {
        case 1:
          dx = unit_length * math::sin(remainder_angle);
          dy = unit_length * math::cos(remainder_angle);
          break;
        case 2:
          dx = unit_length * math::cos(remainder_angle);
          dy = -unit_length * math::sin(remainder_angle);
          break;
        case 3:
          dx = -unit_length * math::sin(remainder_angle);
          dy = -unit_length * math::cos(remainder_angle);
          break;
        case 4:
          dx = -unit_length * math::cos(remainder_angle);
          dy = unit_length * math::sin(remainder_angle);
          break;
      }
      point next_point;
      next_point.x = last_point.x + dx;
      next_point.y = last_point.y + dy;
      //printf("Projected point: %f, %f\n\n", next_point.x, next_point.y);
      return next_point;
    }

    void generate_tree(const char* string) {
      branches_mesh.clear();
      leaves_mesh.clear();
      for (char ch = *string; ch != 0; ch = *(++string)) {
        //printf("%c", ch);

        if (ch == '1' || ch == 'F' || ch == 'A' || ch == 'B') {
          point next_point = get_next_projected_point();
          branches_mesh.push_back(last_point.x);
          branches_mesh.push_back(last_point.y);
          branches_mesh.push_back(next_point.x);
          branches_mesh.push_back(next_point.y);
          //draw_line(last_point, next_point);
          line l;
          l.start = last_point;
          l.end = next_point;
          l.r = 0.4f;
          l.g = 0.3f;
          l.b = 0;
          all_lines.push_back(l);
          last_point = next_point;
          all_points.push_back(last_point);
        }
        else if (ch == '0' || ch == 'X') {
          point next_point = get_next_projected_point();
          //draw_line(last_point, next_point);
          make_leaf(last_point, next_point);
          /*
          line l;
          l.start = last_point;
          l.end = next_point;
          l.r = 0.1f;
          l.g = 0.6f;
          l.b = 0.1f;
          all_lines.push_back(l);
          */
          last_point = next_point;
          all_points.push_back(last_point);
        }
        // Create junction and save current bearing.
        else if (ch == '[') {
          bearing current;
          current.pos = last_point;
          current.angle = facing_angle;
          point_stack.push_back(current);
          facing_angle -= pp_angle;
        }
        else if (ch == ']') {
          // Reset back to junction
          bearing prev = point_stack.back();
          point_stack.pop_back();
          last_point = prev.pos;
          facing_angle = prev.angle;
          facing_angle += pp_angle;
        }
        else if (ch == '+') {
          facing_angle += angle;
        }
        else if (ch == '-') {
          facing_angle -= angle;
        }
      }
    }

    void make_leaf(point start, point end) {
      // Make diamond leaf.
      // dx dy are the vector from start to end.
      float dx = end.x - start.x;
      float dy = end.y - start.y;
      // Mid point of our vector
      point mid;
      mid.x = start.x + dx * 0.5f;
      mid.y = start.y + dy * 0.5f;
      // Normal vector to our line vector.
      // | 0 -1 | normal vector transform
      // | 1  0 | 
      point normal;
      normal.x = -dy;
      normal.y = dx;
      point vert1;
      vert1.x = mid.x + normal.x * 0.25f;
      vert1.y = mid.y + normal.y * 0.25f;
      point vert2;
      vert2.x = mid.x - normal.x * 0.25f;
      vert2.y = mid.y - normal.y * 0.25f;

      leaves_mesh.push_back(start.x);
      leaves_mesh.push_back(start.y);

      leaves_mesh.push_back(vert1.x);
      leaves_mesh.push_back(vert1.y);

      leaves_mesh.push_back(vert2.x);
      leaves_mesh.push_back(vert2.y);


      leaves_mesh.push_back(end.x);
      leaves_mesh.push_back(end.y);

      leaves_mesh.push_back(vert2.x);
      leaves_mesh.push_back(vert2.y);

      leaves_mesh.push_back(vert1.x);
      leaves_mesh.push_back(vert1.y);

      
      triangle tri;
      tri.p1 = start;
      tri.p2 = vert1;
      tri.p3 = vert2;
      all_leaves.push_back(tri);
      tri.p1 = vert2;
      tri.p2 = vert1;
      tri.p3 = end;
      all_leaves.push_back(tri);
    }

    bool is_tree_in_view() {
      for (std::vector<point>::iterator itt = all_points.begin(); itt != all_points.end(); ++itt) {
        if (!is_in_range(*itt)) {
          return false;
        }
      }
      return true;
    }

    void draw_lines() {
      for (std::vector<line>::iterator itt = all_lines.begin(); itt != all_lines.end(); ++itt) {
        line l = *itt;
        glColor3f(l.r, l.g, l.b);
        draw_line(l.start, l.end);
        glColor3f(0, 0, 0);
      }
    }

    void draw_line(point start, point end) {
      glLineWidth(1);
      glBegin(GL_LINES);
      glVertex2f(start.x, start.y);
      glVertex2f(end.x, end.y);
      glEnd();
    }

    void draw_leaves() {
      glColor3f(0, 1, 0);
      glBegin(GL_TRIANGLES);
      for (std::vector<triangle>::iterator itt = all_leaves.begin(); itt != all_leaves.end(); ++itt) {
        triangle tri = *itt;
        glVertex2f(tri.p1.x, tri.p1.y);
        glVertex2f(tri.p2.x, tri.p2.y);
        glVertex2f(tri.p3.x, tri.p3.y);
      }
      glEnd();
    }

    bool is_in_range(point p) {
      return (p.x >= -1 && p.x <= 1 && p.y >= -1 && p.y <= 1);
    }

    void reset() {
      facing_angle = 0;
      last_point = origin;
      all_lines.clear();
      all_points.clear();
      all_leaves.clear();
      all_points.push_back(last_point);

      branches_mesh.clear();
      leaves_mesh.clear();
    }

  public:
    Turtle() {
      code_length = 0;
      unit_length = 0.5f;
      origin.x = 0;
      origin.y = -1;
      angle = pp_angle = 0;
    }

    void clear() {
      code_length = 0;
      unit_length = 0.5f;
      origin.x = 0;
      origin.y = -1;
      angle = pp_angle = 0;
      reset();
    }

    void force_generate() {
      code_length = 0;
      unit_length = 0.5f;
    }

    void get_control_angles(float &a, float &pp_a) {
      a = angle;
      pp_a = pp_angle;
    }

    // a is the angle generated by - and +
    // pp_a is the angle generated by [ and ]
    void set_control_angles(float a, float pp_a) {
      angle = a;
      pp_angle = pp_a;
    }

    void set_origin(float x, float y) {
      origin.x = x;
      origin.y = y;
    }

    void get_origin(float &x, float &y) {
      x = origin.x;
      y = origin.y;
    }

    std::vector<GLfloat> get_branches_mesh() {
      return branches_mesh;
    }

    std::vector<GLfloat> get_leaves_mesh() {
      return leaves_mesh;
    }

    // tree here is the fractal we want to produce
    void render(const char* code) {
      if (strlen(code)>code_length) {
        code_length = strlen(code);
        // Only loop to a maximum of 20 tries 
        unsigned int loop = 0;
        while (loop < 20) {
          ++loop;
          reset();

          generate_tree(code);

          if (!is_tree_in_view()) {
            unit_length = unit_length * 0.5f;
          }
          else {
            break;
          }
        }
        printf("Generated new graphic\n");
      }
      //draw_lines();
      //draw_leaves();
    }
  };
}