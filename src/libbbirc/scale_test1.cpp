#include <stdlib.h>
#include <sstream>
#include "bb_exception.h"
#include "irc_connection.h"

int main(){
  //event_init();

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
  /*
    BeatBoard::IRCConnection a("a");
    a.connectIRCServer(std::string("localhost"),6667);
    a.joinIRCChannel(std::string("#beatboard"));
    a.privMSG( std::string("#beatboard"), std::string( "hogera-" ) );
    BeatBoard::IRCConnection b("b");
    b.connectIRCServer(std::string("localhost"),6667);
    b.joinIRCChannel(std::string("#beatboard"));
    b.privMSG( std::string("#beatboard"), std::string( "hogera-" ) );
  */
  BeatBoard::IRCConnection::bb_event_dispatch();
  //event_dispatch();
  
  return 0;
}
