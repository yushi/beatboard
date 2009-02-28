#include <string.h>
#include <stdlib.h> 
#include <stdio.h> 
%%{ 
  machine foo;

  action start_command { command_start = fpc; }
  action end_command { command_end = fpc; }
  action start_prefix { prefix_start = fpc; }
  action end_prefix { prefix_end = fpc; }
  action start_param { param_start = fpc; }
  action end_param {
    param_end = fpc;
    params[param_num] = get_substr(param_start, param_end);
    param_num++;
  }
  
  irc_nonwhite = ^(0x20 | 0x0 | 0xd | 0xa);
  irc_special = '-' | '[' | ']' | '\\' | '`' | '^' | '{' | '}';
  irc_number = digit;
  irc_letter = alpha;
  irc_user = irc_nonwhite irc_nonwhite*;
  irc_chstring = ^(0x20| 0x07 | 0x0 | 0xd | 0xa | ',');
  irc_mask = ('#' | '$') irc_chstring;
  irc_nick = irc_letter (irc_letter | irc_number| irc_special)*;
  hostname_name = irc_letter (irc_letter | irc_number | '-')* (irc_letter | irc_number);
  irc_hostname = hostname_name ('.' hostname_name)*;
  irc_servername = irc_hostname;
  irc_channel = ('#' | '&') irc_chstring;
  irc_to = irc_channel | irc_user '@' irc_servername | irc_nick | irc_mask;
  irc_crlf = 0xd 0xa;
  irc_space = ' '+;
  irc_trailing = zlen | (irc_space | irc_nonwhite )+;
  irc_middle = ^(':'|0x20|0x0|0xd|0xa) irc_nonwhite*;
  irc_params = (0x20 irc_middle >start_param %end_param)* (0x20 ':' irc_trailing >start_param %end_param )?;
  irc_params_act = irc_params;
  irc_command = irc_letter+ | irc_number irc_number irc_number;
  irc_command_act = irc_command  >start_command %end_command;
  irc_prefix = irc_servername | (irc_nick ('!' irc_user)? ('@' irc_hostname)?);
  irc_prefix_act = irc_prefix >start_prefix %end_prefix;
  irc_message = (':' irc_prefix_act irc_space)? irc_command_act irc_params_act irc_crlf;

  main := 
    irc_message
      0 @{ res = 1; }; 
}%% 
%% write data;

char* get_substr(char* start, char* end){
  char *ret = (char*)malloc(sizeof(char) * (end - start));
  memcpy(ret, start, end - start);
  ret[end - start] = 0;
  return ret;
  
}
int parse_irc_message(char message[]){
  int cs, res = 0;
  char *p = message;
  char *pe = p + strlen(p) + 1;
  char *command_start, *command_end;
  command_start = command_end = 0;
  char *prefix_start, *prefix_end;  
  prefix_start = prefix_end = 0;
  char *param_start, *param_end;
  int param_num = 0;
  param_start = param_end = 0;
  char *params[15];
  for(int i = 0; i < 15; i++){
    params[i] = 0;
  }
  %% write init; 
  %% write exec;
  
  if(command_start && command_end){
    printf("COMMAND: %s\n", get_substr(command_start, command_end) );
  }
  if(prefix_start && prefix_end){
    printf("PREFIX: %s\n", get_substr(prefix_start, prefix_end) );
  }

  for (int i = 0; i < param_num; i++){
    printf("PARAM%2d: %s\n", i, params[i] );
  }
  return 0;
}
int main( int argc, char **argv ) 
{
  parse_irc_message("PRIVMSG #channel text\r\n");
  parse_irc_message("PRIVMSG #channel :text hoge\r\n");
  parse_irc_message(":example.com PRIVMSG #channel text\r\n");
  return 0; 
} 

