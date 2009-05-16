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
#include <map>

using namespace std;
namespace BeatBoard{

  class Notifier{
  public:
    virtual ~Notifier();
    virtual void notify(void* arg);
  };
  /*
   * IRC Connection Class
   */
  class IRCConnection{
    // fields /////////////////////////////////////////////
  public:
    int sock;
    struct bufferevent *buffevent;
    map<string, string> received;
  private:
    string nick;
    static string newline;
    Notifier* notifier;
    // methods ////////////////////////////////////////////
  public:
    static bool bb_event_dispatch(struct event_base *ev);
    static void bb_event_finish();
    IRCConnection(string nick);
    ~IRCConnection();
    void setNotifier(Notifier* notifier);
    Notifier* getNotifier();
    bool hasMessage();
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
