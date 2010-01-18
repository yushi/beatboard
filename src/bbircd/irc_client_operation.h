#ifndef __IRC_CLIENT_OPERATION__
#define __IRC_CLIENT_OPERATION__
#include <map>
#include <string>
#include <irc_connection.h>
#include "bblogger.h"
namespace BeatBoard {
  class IRCClientOperation {
  public:
    IRCClientOperation();
    virtual ~IRCClientOperation();
    void setIRCConnection(std::string nick, IRCConnection* conn);
    IRCConnection* getIRCConnection(std::string nick);
  private:
    std::map<std::string, IRCConnection*> ircConnections;
  };
}

#endif /* __IRC_CLIENT_OPERATION__ */
