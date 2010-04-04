#include "irc_client_operation.h"

using namespace std;
BeatBoard::IRCClientOperation::IRCClientOperation() {
}

BeatBoard::IRCClientOperation::~IRCClientOperation() {
}

string BeatBoard::IRCClientOperation::setIRCConnection(string nick, IRCConnection* conn) {
  string session_id = this->createSessionId();
  this->session2ircConnection[session_id] = conn;
  this->session2nick[session_id] = nick;
  this->nick2session[nick] = session_id;
  
  return session_id;
}

BeatBoard::IRCConnection* BeatBoard::IRCClientOperation::getIRCConnection(string session_id) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  IRCConnection* conn = this->session2ircConnection[session_id];

  if (conn == NULL) {
    logger.debug(session2nick[session_id] + " has not active connection");
  }

  return conn;
}

string BeatBoard::IRCClientOperation::createSessionId() {
  uuid_t uuid;
  char buff[1024];
  uuid_generate(uuid);
  uuid_unparse_upper(uuid, buff);
  return string(buff);
}

bool BeatBoard::IRCClientOperation::isExistNick(string nick){
  string empty = string();
  return (this->nick2session[nick] != empty) ? true : false;
}

string BeatBoard::IRCClientOperation::getSessionByNick(string nick){
  return this->nick2session[nick];
}

