#include <stdio.h>
#include <bb_exception.h>
#include "bblogger.h"
#include <pthread.h>
#include "irc_client_operation.h"
#include "http_api_server.h"

char *http_api_server_addr = "0.0.0.0";
int http_api_server_port = 8000;

namespace BeatBoard{
  namespace BBIRCD{
    class Daemon{
    public:
      void startService();
      void setUp();
      IRCClientOperation ircClients;
    private:
      HTTPAPIServer *httpd;

    };
  }
}
