#include "irc_client_operation.h"
using namespace std;
BeatBoard::IRCClientOperation::IRCClientOperation(){
}

BeatBoard::IRCClientOperation::~IRCClientOperation(){
}

void BeatBoard::IRCClientOperation::setIRCConnection(string nick, IRCConnection* conn){
  this->ircConnections[nick] = conn;
}

BeatBoard::IRCConnection* BeatBoard::IRCClientOperation::getIRCConnection(string nick){
  IRCConnection* conn = this->ircConnections[nick];
  if(conn == NULL){
    cout << "null detected!!!!!!!!!!!!!!!!!!" << endl;
  }
  return conn;
}
