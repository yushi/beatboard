#include <stdlib.h>
#include <sys/queue.h>
#include <event.h>
#include <evhttp.h>
#include <iostream>
#include "irc_client_operation.h"
#include "bblogger.h"
namespace BeatBoard{
  class HTTPAPIServer  : public IRCClientOperation {
    //class HTTPAPIServer{
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
    struct evhttp *httpd;
    map<string, string> parseParameter(char* uri);
  };

 class HTTPAPINotifier: public Notifier {
  public:
    struct evhttp_request* req;
    struct evbuffer* buf;
    IRCConnection* conn;
    HTTPAPINotifier(struct evhttp_request* req, IRCConnection* conn);
    ~HTTPAPINotifier();
    void notify(void* arg);
  };
}
