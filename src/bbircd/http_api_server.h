#include <stdlib.h>
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
    static void joinHandler( struct evhttp_request *req, void *arg );
    static void speakHandler( struct evhttp_request *req, void *arg );    
  private:
    struct evhttp *httpd;
    struct event_base *http_ev_base;
  };
}