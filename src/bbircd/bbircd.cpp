#include "bbircd.h"

void sig_handler_SIGPIPE(int sig){
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug( "SIGPIPE received" );
}

void BeatBoard::BBIRCD::Daemon::setUp() {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();

  try {
    this->httpd = new HTTPAPIServer();
    this->httpd->setUp( http_api_server_addr, http_api_server_port );
  } catch ( BeatBoard::Exception& error ) {
    logger.debug( error.message.data() );
    std::cerr << "daemon start failed.\n";
  }

  return;
}

void BeatBoard::BBIRCD::Daemon::startService() {
  while ( !IRCConnection::bb_event_dispatch( this->httpd->http_ev_base ) ) {
    fprintf( stderr, "start fail\n" );
    sleep( 1 );
  }

  cout << "event loop exited" << endl;
}

void unexpected_exception_handler( void ) {
  std::cout << "this is unexcepted Exception handler\n";
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug( "unknown exception throwed" );
}

int main( int argc, char** argv ) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.info( "bbircd started" );
  std::set_unexpected( unexpected_exception_handler );

  signal(SIGPIPE , sig_handler_SIGPIPE);
  BeatBoard::BBIRCD::Daemon bbircd;
    bbircd.setUp();
    bbircd.startService();

    return 0;
}
