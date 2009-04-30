#include "irc_client_operation.h"

BeatBoard::IRCClientOperation::IRCClientOperation(){
}

BeatBoard::IRCClientOperation::~IRCClientOperation(){
}

void BeatBoard::IRCClientOperation::setIRCConnection(std::string nick, IRCConnection* conn){
  //FIXME fixed hash key
  //this->ircConnections.insert( std::map<std::string, IRCConnection*>::value_type( nick, conn ) );
  this->ircConnections[nick] = conn;
}

BeatBoard::IRCConnection* BeatBoard::IRCClientOperation::getIRCConnection(std::string nick){
  return this->ircConnections[nick];
}
