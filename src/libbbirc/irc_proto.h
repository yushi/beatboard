#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>

namespace BeatBoard{
  class IRCEvent{
  public:
    std::string *prefix;
    std::string *command;
    std::string *params[15];
    int param_num;
    IRCEvent();
    ~IRCEvent();
    std::string inspect();
  };  

  IRCEvent* parse_irc_message(char message[]);
}
