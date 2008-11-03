#include <stdio.h>
#include "irc_connection.h"
#include "bb_exception.h"
#include "log4cpp_def.h"
#include "bblogger.h"
#include "http_api_server.h"

namespace BeatBoard{
  class BBIRCD{
  public:
    void startService();
    void setUp();
  private:
    HTTPAPIServer *httpd;
  };
}
