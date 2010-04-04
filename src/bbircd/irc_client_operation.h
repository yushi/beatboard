#ifndef __IRC_CLIENT_OPERATION__
#define __IRC_CLIENT_OPERATION__
#include <map>
#include <string>
#include <irc_connection.h>
#include "bblogger.h"
#include <uuid/uuid.h>

namespace BeatBoard {
  class IRCClientOperation {
  public:
    IRCClientOperation();
    virtual ~IRCClientOperation();
    // set connection by nick and returns session_id
    string setIRCConnection(std::string nick, IRCConnection* conn);
    IRCConnection* getIRCConnection(std::string nick);
  private:
    string createSessionId();
    std::map<std::string, IRCConnection*> session2ircConnection; // session id to IRCConnection map
    std::map<std::string, std::string> session2nick; // session id to nick map
  };
}

#endif /* __IRC_CLIENT_OPERATION__ */
