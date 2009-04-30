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
#include "irc_proto.h"

using namespace std;
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
    string nick;
    static string newline;
    // methods ////////////////////////////////////////////
  public:
    static bool bb_event_dispatch();
    static void bb_event_finish();
    IRCConnection(string nick);
    ~IRCConnection();
    void connectIRCServer(string addr, string port) throw (Exception);
    void disconnectIRCServer(void) throw (Exception);
    void JOIN(string channel) throw (Exception);
    void PRIVMSG(string channel, string msg) throw (Exception);
    void PONG(string server) throw (Exception);
    void NICK(string name) throw (Exception);
    void USER(string user, string host, string server, string real) throw (Exception);
  private:
    void write(string str) throw (Exception);
    void create_socket(void) throw (Exception);
  };
}
#endif /* __IRC_CONNECTION__ */
