#include "irc_client_operation.h"
#include <irc_connection.h>
#include <iostream>

using namespace std;
using namespace BeatBoard;
int main(void){
  IRCClientOperation a;
  IRCConnection *b = new IRCConnection(string("test"));

  a.setIRCConnection(string("hoge"),b);
  cout <<"hgoehoge" << endl;
}
