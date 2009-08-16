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
#include <vector>
#include "bb_exception.h"
#include "irc_proto.h"
#include <map>
#include "bb_protobuf_queue_memcached.h"
#include "logapi.pb.h"

using namespace std;
namespace BeatBoard{

  class Notifier{
  public:
    virtual ~Notifier();
    virtual bool notify(map<string, vector<string> >* arg);
  };
  /*
   * IRC Connection Class
   */
  class IRCConnection{
    // fields /////////////////////////////////////////////
  public:
    int sock;
    struct bufferevent *buffevent;
    map<string, vector<string> > received;
  private:
    string nick;
    static string newline;
    vector<Notifier*> notifier;
    // methods ////////////////////////////////////////////
  public:
    static bool bb_event_dispatch(struct event_base *ev);
    static void bb_event_finish();
    IRCConnection(string nick);
    ~IRCConnection();
    void setNotifier(Notifier* notifier);
    void notify();
    bool hasMessage();
    map<string, vector<string> > getMessage();
    void connectIRCServer(string addr, string port) throw (Exception);
    void disconnectIRCServer(void) throw (Exception);
    void JOIN(string channel) throw (Exception);
    void PRIVMSG(string channel, string msg) throw (Exception);
    void PONG(string server) throw (Exception);
    void NICK(string name) throw (Exception);
    void USER(string user, string host, string server, string real) throw (Exception);
    void loggingMessage(string channel, string identifier, string message);
  private:
    void write(string str) throw (Exception);
    void create_socket(void) throw (Exception);

  };

}
#endif /* __IRC_CONNECTION__ */
