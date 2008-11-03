#include <map>
#include <string>
#include "irc_connection.h"

namespace BeatBoard{
  class IRCClientOperation{
  public:
    IRCClientOperation();
    virtual ~IRCClientOperation();
    void setIRCConnection(IRCConnection* conn);
    IRCConnection* getIRCConnection(std::string nick);
  private:
    std::map<std::string, IRCConnection*> ircConnections;
  };
}
