#include "bbircd.h"

void BeatBoard::BBIRCD::setUp() {
  event_init();
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  /*
  try {
    this->httpd = new HTTPAPIServer();
    this->httpd->setUp(http_api_server_addr, http_api_server_port);
  } catch ( BeatBoard::Exception& error ) {
    logger.debug( error.message.data() );
    std::cerr << "daemon start failed.\n";
  }
  */
  return;
}

void BeatBoard::BBIRCD::startService(){
  event_dispatch();
}

void unexpected_exception_handler( void ) {
  std::cout << "this is unexcepted Exception handler\n";
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug( "unknown exception throwed" );
}

int main( int argc, char** argv ) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.info("bbircd started");
  std::set_unexpected( unexpected_exception_handler );

  BeatBoard::BBIRCD bbircd;
  bbircd.setUp();
  bbircd.startService();

  return 0;
}
