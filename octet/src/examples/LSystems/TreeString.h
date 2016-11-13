

namespace octet {
  class TreeString {
  private:
    //char* current;
    //std::map<char*, char*> conversion_table;

    //std::vector<char[]> history;

    std::string current;
  public:
    TreeString(){}

    void set_axiom(char * axiom) {
      current = axiom;
    }

    const char* get_string() {
      return current.data();
    }

    void do_step() {
      std::string newString;
      /*for (std::string::iterator itt = current.begin(); itt != current.end(); ++itt) {
        printf("%c", *itt);

        if (*itt == '0') {
          newString += "1[0]0";
        }
        else if (*itt == '1') {
          newString += "11";
        }
        else {
          newString += *itt;
        }
      }*/

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
      }

      current = newString;

      //printf("\nResult: %s\n", newString.c_str());
      
      /*
      char buffer[2048];
      int buffer_index = 0;

      char* itt = current;
      for (char ch = *itt; ch != 0; ch = *(++itt)) {
        printf(" - %c\n", ch);
        if (ch == '1') {
          buffer[buffer_index++] = '1';
          buffer[buffer_index++] = '1';
        }
        else if (ch == '0') {
          buffer[buffer_index++] = '1';
          buffer[buffer_index++] = '['; 
          buffer[buffer_index++] = '0';
          buffer[buffer_index++] = ']'; 
          buffer[buffer_index++] = '0';
        }
        else if(ch == '[') {
          buffer[buffer_index++] = '[';
        }
        else if (ch == ']') {
          buffer[buffer_index++] = ']';
        }

      }
      buffer[buffer_index++] = 0;

      char* itt2 = current;
      printf("Preparing to render: ");
      for (char ch = *itt2; ch != 0; ch = *(++itt2)) {
        printf("%c", ch);
      }
      printf("\n");

      */
    }
  };
}