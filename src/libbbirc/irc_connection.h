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
#include <sstream>
#include "bb_exception.h"

using namespace std;
namespace BeatBoard{
  struct event_base *ev_base = NULL;
  /*
   * IRC Connection Class
   */
  class IRCConnection{
    // fields /////////////////////////////////////////////
  public:
    int sock;
    struct bufferevent *buffevent;
  private:
    string nick;
    static string newline;
    static int is_initialized;
    // methods ////////////////////////////////////////////
  public:
    static void bb_event_dispatch();
    static void bb_event_finish();
    IRCConnection(string nick);
    ~IRCConnection();
    void connectIRCServer(string addr, string port) throw (Exception);
    void joinIRCChannel(string channel) throw (Exception);
    void privMSG(string channel, string msg) throw (Exception);
  private:
    void create_socket(void) throw (Exception);
  };
};
#endif /* __IRC_CONNECTION__ */
