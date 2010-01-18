#include "irc_client_operation.h"

using namespace std;
BeatBoard::IRCClientOperation::IRCClientOperation() {
}

BeatBoard::IRCClientOperation::~IRCClientOperation() {
}

void BeatBoard::IRCClientOperation::setIRCConnection(string nick, IRCConnection* conn) {
  this->ircConnections[nick] = conn;
}

BeatBoard::IRCConnection* BeatBoard::IRCClientOperation::getIRCConnection(string nick) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  IRCConnection* conn = this->ircConnections[nick];

  if (conn == NULL) {
    logger.debug(nick + " has not active connection");
  }

  return conn;
}
