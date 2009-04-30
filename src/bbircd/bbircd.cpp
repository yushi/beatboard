#include "bbircd.h"

pthread_t pt;

void* evqueue_listener(void *arg){
  BeatBoard::BBIRCD::EventQueue queue;
  BeatBoard::BBIRCD::Daemon *bbircd;
  bbircd = (BeatBoard::BBIRCD::Daemon*)arg;
  while(1){
    BeatBoard::BBIRCD::Event ev = queue.dequeue();

    if((int)ev.type() == (int)BeatBoard::BBIRCD::TYPE_CONNECT ){
      string server = ev.value(0);
      string nick = ev.value(1);
      string port = ev.value(2);
      cout << server << endl;
      cout << nick << endl;
      cout << port << endl;

      BeatBoard::IRCConnection *testConn;
      testConn = new BeatBoard::IRCConnection(nick);
      testConn->connectIRCServer(server, port);
      bbircd->ircClients.setIRCConnection(nick,testConn);
    }else if((int)ev.type() == (int)BeatBoard::BBIRCD::TYPE_JOIN){
      string channel = ev.value(0);
      string nick = ev.value(1);
      
      cout << channel << endl;
      cout << nick << endl;

      BeatBoard::IRCConnection *testConn = bbircd->ircClients.getIRCConnection(nick);
      testConn->JOIN(channel);      

    }else if((int)ev.type() == (int)BeatBoard::BBIRCD::TYPE_PRIVMSG ){
      string target = ev.value(0);
      string message = ev.value(1);
      string nick = ev.value(2);

      BeatBoard::IRCConnection *testConn = bbircd->ircClients.getIRCConnection(nick);
      testConn->JOIN(target);      
      testConn->PRIVMSG(target, message);
    }
    
  }
}

void BeatBoard::BBIRCD::Daemon::setUp() {
  //event_init();
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

  pthread_create( &pt, NULL, &evqueue_listener, this );
  return;
}

void BeatBoard::BBIRCD::Daemon::startService(){
  while(!IRCConnection::bb_event_dispatch()){
    sleep(1);
  }
  cout << "event loop exited" << endl;
  pthread_join(pt,NULL);
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

  BeatBoard::BBIRCD::Daemon bbircd;
  bbircd.setUp();
  bbircd.startService();

  return 0;
}
