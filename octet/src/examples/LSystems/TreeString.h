

namespace octet {
  class TreeString {
  private:
    //char* current;
    std::map<char, std::string> conversion_table;

    //std::vector<char[]> history;

    std::string current;

    bool is_control_char(char ch) {
      for (std::map<char, std::string>::iterator it = conversion_table.begin(); it != conversion_table.end(); ++it) {
        //printf(" - Key: %c\n", *it);
        if (it->first == ch) return true;
      }
      return false;
    }

  public:
    TreeString(){}

    void set_axiom(const char * axiom) {
      //printf("Axiom set to %s\n", axiom);
      current = axiom;
    }

    void clear_rules() {
      conversion_table.clear();
    }

    void insert_rule(char key, std::string value) {
      conversion_table.insert(std::pair<char, std::string>(key, value));
      //printf("Creating key->val pair: %c -> %s\n", key, conversion_table.find(key)->second.c_str());
    }

    const char* get_string() {
      return current.data();
    }

    void do_step() {
      std::string newString;

      for (std::string::iterator itt = current.begin(); itt != current.end(); ++itt) {
        if (is_control_char(*itt)) {
          //printf("EVOLVE converting %c to %s\n", *itt, conversion_table.find(*itt)->second.c_str());
          newString += conversion_table.find(*itt)->second;
        }
        else {
          //printf("EVOLVE passing on %c\n", *itt);
          newString += *itt;
        }
      }
      /*
      for (std::string::iterator itt = current.begin(); itt != current.end(); ++itt) {
        //printf("%c", *itt);

        if (*itt == '0') {
          newString += "1[0]0";
        }
        else if (*itt == '1') {
          newString += "11";
        }
        else {
          newString += *itt;
        }
      }
      */
      /*
      for (std::string::iterator itt = current.begin(); itt != current.end(); ++itt) {
        //printf("%c", *itt);

        if (*itt == 'A') {
          newString += "+B-A-B+";
        }
        else if (*itt == 'B') {
          newString += "-A+B+A-";
        }
        else {
          newString += *itt;
        }
      }*/

      current = newString;

      //printf("\nResult: %s\n", newString.c_str());
    }
  };
}