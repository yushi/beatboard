#include <event.h>
#include <evhttp.h>
#include "bblogger.h"
#include <sstream>
#include "irc_client_operation.h"
namespace BeatBoard {

  class HTTPAPIReadNotifier: public Notifier {
  public:
    struct evhttp_request* req;
    struct evbuffer* buf;
    time_t init_time;
    IRCConnection* conn;
    HTTPAPIReadNotifier(struct evhttp_request* req);
    ~HTTPAPIReadNotifier();
    bool notify(map<string, vector<string> >* arg);
  private:
    string escape(string str);
  };
}
