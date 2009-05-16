#include "irc_client_operation.h"

BeatBoard::IRCClientOperation::IRCClientOperation(){
}

BeatBoard::IRCClientOperation::~IRCClientOperation(){
}

void BeatBoard::IRCClientOperation::setIRCConnection(std::string nick, IRCConnection* conn){
  this->ircConnections[nick] = conn;
}

BeatBoard::IRCConnection* BeatBoard::IRCClientOperation::getIRCConnection(std::string nick){
  return this->ircConnections[nick];
}
