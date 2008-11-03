#include "irc_client_operation.h"

BeatBoard::IRCClientOperation::IRCClientOperation(){
}

BeatBoard::IRCClientOperation::~IRCClientOperation(){
}

void BeatBoard::IRCClientOperation::setIRCConnection(IRCConnection* conn){
  //FIXME fixed hash key
  this->ircConnections.insert( std::map<std::string, IRCConnection*>::value_type( "yushi_bbircd", conn ) );
}

BeatBoard::IRCConnection* BeatBoard::IRCClientOperation::getIRCConnection(std::string nick){
  return this->ircConnections[nick];
}
