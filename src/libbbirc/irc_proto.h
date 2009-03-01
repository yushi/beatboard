#include <string.h>
#include <stdlib.h> 
#include <stdio.h>
#include <string>
#include <iostream>


class IRCEvent{
 public:
  std::string *prefix;
  std::string *command;
  std::string *params[15];
  int param_num;
  IRCEvent(){
    this->param_num = 0;
    this->prefix = NULL;
    this->command = NULL;
    for(int i = 0; i < 15; i++){
      this->params[i] = NULL;
    }
  }
  void print(){
    if(this->prefix){
      std::cout << "PREFIX: " <<  *(this->prefix) << std::endl;
    }
    if(this->command){
      std::cout << "COMMAND: " << *(this->command) << std::endl;
    }
    for (int i = 0; i < param_num; i++){
      std::cout << "PARAM: " << *(this->params[i]) << std::endl;
    }
  }
};  
  
IRCEvent* parse_irc_message(char message[]);
