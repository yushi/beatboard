#include <stdio.h>
#include <getopt.h>
#include <bb_exception.h>
#include "bblogger.h"
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "http_api_server.h"
#include "session_manager.h"

namespace BeatBoard{
  namespace BBIRCD{
    class Daemon{
    public:
      void startService();
      void setUp(char* addr, int port, int timeout);
      SessionManager ircClients;
    private:
      HTTPAPIServer *httpd;

    };
  }
}



