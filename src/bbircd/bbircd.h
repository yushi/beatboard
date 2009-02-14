#include <stdio.h>
#include <irc_connection.h>
#include <bb_exception.h>
#include "bblogger.h"
#include "http_api_server.h"

char *http_api_server_addr = "0.0.0.0";
int http_api_server_port = 8000;

namespace BeatBoard{
  class BBIRCD{
  public:
    void startService();
    void setUp();
  private:
    HTTPAPIServer *httpd;
  };
}
