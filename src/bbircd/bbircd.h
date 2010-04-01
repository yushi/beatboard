#include <stdio.h>
#include <unistd.h>
#include <bb_exception.h>
#include "bblogger.h"
#include <pthread.h>
#include <signal.h>
#include "irc_client_operation.h"
#include "http_api_server.h"

namespace BeatBoard{
  namespace BBIRCD{
    class Daemon{
    public:
      void startService();
      void setUp(char* addr, int port);
      IRCClientOperation ircClients;
    private:
      HTTPAPIServer *httpd;

    };
  }
}
