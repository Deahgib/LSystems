
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

    float facing_angle;
    float unit_length;
    float angle;

    std::vector<point> all_points;
    std::vector<line> all_lines;
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
      float remainder_angle = (int)facing_angle % 90;
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
      for (char ch = *string; ch != 0; ch = *(++string)) {
        //printf("%c", ch);

        if (ch == '1' || ch == 'F' || ch == 'A' || ch == 'B') {
          point next_point = get_next_projected_point();
          //draw_line(last_point, next_point);
          line l;
          l.start = last_point;
          l.end = next_point;
          l.r = 0.1f;
          l.g = 0.3f;
          l.b = 0.1f;
          all_lines.push_back(l);
          last_point = next_point;
          all_points.push_back(last_point);
        }
        else if (ch == '0') {
          point next_point = get_next_projected_point();
          //draw_line(last_point, next_point);
          line l;
          l.start = last_point;
          l.end = next_point;
          l.r = 0.1f;
          l.g = 0.6f;
          l.b = 0.1f;
          all_lines.push_back(l);
          last_point = next_point;
          all_points.push_back(last_point);
        }
        // Create junction and save current bearing.
        else if (ch == '[') {
          bearing current;
          current.pos = last_point;
          current.angle = facing_angle;
          point_stack.push_back(current);
          facing_angle -= angle;
        }
        else if (ch == ']') {
          // Reset back to junction
          bearing prev = point_stack.back();
          point_stack.pop_back();
          last_point = prev.pos;
          facing_angle = prev.angle;
          facing_angle += angle;
        }
        else if (ch == '+') {
          facing_angle += angle;
        }
        else if (ch == '-') {
          facing_angle -= angle;
        }
      }
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
      glBegin(GL_LINES);
      glVertex2f(start.x, start.y);
      glVertex2f(end.x, end.y);
      glEnd();
    }

    bool is_in_range(point p) {
      return (p.x >= -1 && p.x <= 1 && p.y >= -1 && p.y <= 1);
    }

  public:
    Turtle() {
      reset();
      unit_length = 0.5f;
      origin.x = 0;
      origin.y = -0.5;
      angle = 60;
    }

    void reset() {
      facing_angle = 0;
      last_point = origin;
      all_lines.clear();
      all_points.clear();
      all_points.push_back(last_point);
    }


    void render(const char* string) {
      
      while (true) {
        reset();

        generate_tree(string);

        if (!is_tree_in_view()) {
          unit_length = unit_length * 0.5f;
        }
        else {
          break;
        }
      }
      draw_lines();
    }
  };
}