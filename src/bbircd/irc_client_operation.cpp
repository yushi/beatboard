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
    map<string, IRCConnection*>::iterator i = this->ircConnections.begin();
    while(i != this->ircConnections.end()){
      cout << i->first << ": " << sizeof(i->first) << endl;
      i++;
    }
  }
  return conn;
}
