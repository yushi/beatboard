#include <stdlib.h>
#include <time.h>
#include <sys/queue.h>
#include <event.h>
#include <evhttp.h>
#include <iostream>
#include <sstream>
#include "irc_client_operation.h"
#include "bblogger.h"
namespace BeatBoard{
  class HTTPAPIServer  : public IRCClientOperation {
  public:
    HTTPAPIServer();
    ~HTTPAPIServer();
    void setUp(char *addr, int port);
    static void rootHandler( struct evhttp_request *req, void *arg );
    static void connectHandler( struct evhttp_request *req, void *arg );
    static void joinHandler( struct evhttp_request *req, void *arg );
    static void exitHandler( struct evhttp_request *req, void *arg );    
    static void speakHandler( struct evhttp_request *req, void *arg );    
    static void readHandler( struct evhttp_request *req, void *arg );
    struct event_base *http_ev_base;
  private:
    static const int TIMEOUT;
    struct evhttp *httpd;
    map<string, string> parseParameter(struct evhttp_request *req);
    map<string, string> parseGetParameter(struct evhttp_request *req);
    map<string, string> parsePostParameter(struct evhttp_request *req);
  };

 class HTTPAPINotifier: public Notifier {
  public:
    struct evhttp_request* req;
    struct evbuffer* buf;
    time_t init_time;
    IRCConnection* conn;
    HTTPAPINotifier(struct evhttp_request* req, IRCConnection* conn);
    ~HTTPAPINotifier();
    void notify(void* arg);
  private:
    string escape(string str);
  };
}
