#include <string.h> 
#include <stdio.h> 
%%{ 
  machine foo;

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
  #irc_target = irc_to "," irc_target;  # nest...
  irc_crlf = 0xd 0xa;
  irc_trailing = (irc_nonwhite* | zlen);
  irc_middle = ^':' irc_nonwhite+;
  #irc_params = 0x20 ('：' irc_trailing | irc_middle irc_params)+; #nest
  irc_space = ' '+;
  irc_command = irc_letter+ | irc_number irc_number irc_number;
  irc_prefix = irc_servername | (irc_nick ('!' irc_user)? ('@' irc_hostname)?);
  #irc_message = (':' irc_prefix irc_space)? irc_command irc_params irc_crlf;



  main := 
    "sample"
      0 @{ res = 1; }; 
}%% 
%% write data;

int main( int argc, char **argv ) 
{

  char str[] = "sample";
  int cs, res = 0; 
  char *p = str;
  char *pe = p + strlen(p) + 1; 
  %% write init; 
  %% write exec; 

  printf("result = %i\n", res ); 
  return 0; 
} 
