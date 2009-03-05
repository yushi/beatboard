#include <stdlib.h>
#include <sstream>
#include "bb_exception.h"
#include "irc_connection.h"

int main(){
  int nums = 1;
  BeatBoard::IRCConnection* connections[nums];
  try{
    for(int i=0; i < nums; i++){
      std::ostringstream stream;
      stream << "hoge" << i;
      connections[i] = new BeatBoard::IRCConnection(stream.str());
      connections[i]->connectIRCServer(std::string("localhost"),6667);
      connections[i]->joinIRCChannel(std::string("#beatboard"));
      connections[i]->privMSG( std::string("#beatboard"), std::string( "hogera-" ) );
    }
  }
  catch(BeatBoard::Exception& e){
    std::cout << e.message << std::endl;
    printf("HOGEHOGE\n");
  }
  BeatBoard::IRCConnection::bb_event_dispatch();
  
  return 0;
}
