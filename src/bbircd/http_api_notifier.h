#ifndef __HTTP_API_NOTIFIER__
#define __HTTP_API_NOTIFIER__

#include <json/json.h>
#include <map>
#include <vector>
#include <event.h>
#include <evhttp.h>
#include <sstream>
#include <irc_connection.h>
#include "bblogger.h"

using namespace std;
namespace BeatBoard {

  class HTTPAPIReadNotifier: public Notifier {
  public:
    int timeout;
    bool isResponsed;
    struct event timeout_timer;
    struct evhttp_request* req;
    struct evbuffer* buf;
    time_t init_time;
    IRCConnection* conn;
    HTTPAPIReadNotifier(struct evhttp_request* req, int timeout);
    ~HTTPAPIReadNotifier();
    void timeout_response();
    bool notify(map<string, vector<string> >* arg);
    bool timeout_timer_enabled;
  };
}

#endif /* __HTTP_API_NOTIFIER__ */
