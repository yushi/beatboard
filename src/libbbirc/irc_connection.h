#ifndef __IRC_CONNECTION__
#define __IRC_CONNECTION__

#include <netdb.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <event.h>
#include <iostream>
#include "bb_exception.h"

namespace BeatBoard{
  /*
   * IRC Connection Class
   */
  class IRCConnection{
    // fields /////////////////////////////////////////////
  public:
    int sock;
    struct bufferevent *buffevent;
  private:
    std::string nick;
    static std::string newline;
    static int is_initialized;
    // methods ////////////////////////////////////////////
  public:
    static void bb_event_dispatch();
    IRCConnection(std::string nick);
    void connectIRCServer(std::string addr, int port) throw (Exception);
    void joinIRCChannel(std::string channel) throw (Exception);
    void privMSG(std::string channel, std::string msg) throw (Exception);
  private:
    void create_socket(void) throw (Exception);
  };
};
#endif /* __IRC_CONNECTION__ */
