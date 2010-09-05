#include <stdlib.h>
#include <time.h>
#include <sys/queue.h>
#include <event.h>
#include <evhttp.h>
#include <iostream>
#include <sstream>
#include "bblogger.h"
#include "session_manager.h"
#include "http_api_notifier.h"


namespace BeatBoard {
  class HTTPAPIServer  : public SessionManager {
  public:
    HTTPAPIServer();
    ~HTTPAPIServer();
    void setUp(char *addr, int port, int timeout);
    void shutdown();
    static void rootHandler(struct evhttp_request *req, void *arg);
    static void connectHandler(struct evhttp_request *req, void *arg);
    static void joinHandler(struct evhttp_request *req, void *arg);
    static void exitHandler(struct evhttp_request *req, void *arg);
    static void speakHandler(struct evhttp_request *req, void *arg);
    static void readHandler(struct evhttp_request *req, void *arg);
    struct event_base *http_ev_base;
  private:
    struct evhttp *httpd;
    static int timeout;
    map<string, string> parseParameter(struct evhttp_request *req);
    map<string, string> parseGetParameter(struct evhttp_request *req);
    map<string, string> parsePostParameter(struct evhttp_request *req);
  };
}
