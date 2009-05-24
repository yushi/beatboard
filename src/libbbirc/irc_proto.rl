#include "irc_proto.h"

%%{ 
  machine foo;

  action start_command { start = fpc; }
  action end_command {
    end = fpc;
    ret->command = get_substr(start, end);
  }
  
  action start_prefix { start = fpc; }
  action end_prefix {
    end = fpc;
    ret->prefix = get_substr(start, end);
  }
  
  action start_param { start = fpc; }
  action end_param {
    end = fpc;
    ret->params[ret->param_num] = get_substr(start, end);
    ret->param_num++;
  }
  
  irc_nonwhite = ^(0x20 | 0x0 | 0xd | 0xa);
  irc_special = '-' | '[' | ']' | '\\' | '`' | '^' | '{' | '}' | '_' | '|';
  #%x5B-60 / %x7B-7D
  irc_number = digit;
  irc_letter = alpha;
  irc_user = irc_nonwhite irc_nonwhite*;
  irc_chstring = ^(0x20| 0x07 | 0x0 | 0xd | 0xa | ',');
  irc_mask = ('#' | '$') irc_chstring;
#  irc_nick = irc_letter (irc_letter | irc_number| irc_special)*;
  irc_nick = (irc_letter | irc_special) (irc_letter | irc_number| irc_special)*;
  hostname_name = irc_letter (irc_letter | irc_number | '-')* (irc_letter | irc_number);
  irc_hostname = hostname_name ('.' hostname_name)*;
  irc_servername = irc_hostname;
  irc_channel = ('#' | '&') irc_chstring;
  irc_to = irc_channel | irc_user '@' irc_servername | irc_nick | irc_mask;
#  irc_crlf = 0xd 0xa;
  irc_crlf = 0xd;
  irc_space = ' '+;
  irc_trailing = zlen | (irc_space | irc_nonwhite )+;
  irc_middle = ^(':'|0x20|0x0|0xd|0xa) irc_nonwhite*;
  irc_params = (0x20 irc_middle >start_param %end_param)* (0x20 ':' irc_trailing >start_param %end_param )?;
  irc_params_act = irc_params;
  irc_command = irc_letter+ | irc_number irc_number irc_number;
  irc_command_act = irc_command  >start_command %end_command;
  irc_prefix = (irc_servername) | (irc_nick ('!' irc_user)? ('@' irc_servername)?);
  irc_prefix_act = irc_prefix >start_prefix %end_prefix;
  irc_message = (':' irc_prefix_act irc_space)? irc_command_act irc_params_act irc_crlf;

  main := 
    irc_message
      0 @{ res = 1; }; 
}%% 
%% write data;

std::string* get_substr(char* start, char* end){
  char *buf = (char*)malloc(sizeof(char) * (end - start + 1));
  std::string *ret;
  memcpy(buf, start, end - start);
  buf[end - start] = 0;
  ret = new std::string(buf);
  free(buf);
  return ret;
  
}

BeatBoard::IRCEvent* BeatBoard::parse_irc_message(char message[]){
  int cs, res = 0;
  char *p = message;
  char *pe = p + strlen(p) + 1;
  char *start, *end;
  start = end = 0;
  BeatBoard::IRCEvent *ret = new BeatBoard::IRCEvent();
  
  %% write init; 
  %% write exec;
  if(res){
    return ret;
  }else{
    delete ret;
    return NULL;
  }
}

BeatBoard::IRCEvent::IRCEvent(){
  this->param_num = 0;
  this->prefix = NULL;
  this->command = NULL;
  for(int i = 0; i < 15; i++){
    this->params[i] = NULL;
  }
}

BeatBoard::IRCEvent::~IRCEvent(){
  if(this->prefix){
    delete this->prefix;
  }

  if(this->command){
    delete this->command;
  }

  for(int i = 0; i < this->param_num; i++){
    delete this->params[i];
  }
}

std::string BeatBoard::IRCEvent::inspect(){
  std::ostringstream o;
  if(this->prefix){
    o << "PREFIX: " <<  *(this->prefix) << std::endl;
  }
  if(this->command){
    o << "COMMAND: " << *(this->command) << std::endl;
  }
  for (int i = 0; i < param_num; i++){
    o << "PARAM: " << *(this->params[i]) << std::endl;
  }
  return o.str();
}
