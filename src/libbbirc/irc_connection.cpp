#include "irc_connection.h"
//FIXME 文字列操作を連結でやるのは汚い
//フォーマット文字列でやる?


// static field
std::string BeatBoard::IRCConnection::newline = std::string("\r\n");
int BeatBoard::IRCConnection::is_initialized = 0;
void BeatBoard::IRCConnection::bb_event_dispatch(){
  event_dispatch();
}

/* readable event handler for libevent */

void irc_buffevent_read( struct bufferevent *bev, void *arg ) {
  char buf[1024];
  while ( 0 < bufferevent_read( bev, buf, 1023 ) ) {
    //std::cout << std::string(buf);
  }
}

/* writable event handler for libevent */

void irc_buffevent_write( struct bufferevent *bev, void *arg ) {
}

/* error event handler for libevent */

void irc_buffevent_error( struct bufferevent *bev, short what, void *arg ) {
  std::cout << "error ev\n";
  //FIXME cleanup bufferevent
}


BeatBoard::IRCConnection::IRCConnection(std::string nick) {
  this->nick = nick;
  if(0 == this->is_initialized){
    this->is_initialized = 1;
    event_init();
  }
}

void BeatBoard::IRCConnection::create_socket() throw( Exception ) {
  this->sock = socket( AF_INET, SOCK_STREAM, 0 );

  if ( this->sock == -1 ) {
    std::string message = std::string( "socket: " );
    message += strerror( errno );
    throw Exception( message );
  }
}

void BeatBoard::IRCConnection::connectIRCServer(std::string addr, std::string port) throw (Exception){
  this->create_socket();

  struct addrinfo hints;
  struct addrinfo *addrinfo = NULL;
  int result = -1;

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  
  result = getaddrinfo(addr.c_str(),port.c_str(),&hints,&addrinfo);
  if ( 0 != result  ) {
    throw Exception( "error: invalid address" );
  }
  
  if ( -1 == connect( this->sock, addrinfo->ai_addr, addrinfo->ai_addrlen ) ) {
    printf( "connect failed" );
    exit( -1 );
  }
  freeaddrinfo(addrinfo);

  /* add event to this connection */
  this->buffevent = bufferevent_new( this->sock, irc_buffevent_read, irc_buffevent_write, irc_buffevent_error, NULL );
  if ( NULL == this->buffevent ) {
    throw Exception( "bufferevent_new: failed" );
  }

  result = bufferevent_enable( this->buffevent, EV_READ | EV_WRITE );

  if ( 0 != result ) {
    throw Exception( "bufferevent_enable: failed" );
  }

  /* send first message (NICK, User) */
  std::string initMessage("NICK ");
  initMessage += this->nick + "\r\nUSER " + this->nick + " 0 * :beatboard\r\n";
  result = bufferevent_write( this->buffevent, initMessage.c_str(), initMessage.length());

  if ( 0 != result ) {
    throw Exception( "bufferevent_write: failed" );
  }
}

void BeatBoard::IRCConnection::joinIRCChannel(std::string channel) throw (Exception){
  std::string message("JOIN ");
  message += channel;
  message += this->newline;
  
  int result;
  result = bufferevent_write( this->buffevent, message.c_str(), message.length() );

  if ( 0 != result ) {
    throw Exception( "bufferevent_write: failed" );
  }
}


void BeatBoard::IRCConnection::privMSG( std::string channel, std::string msg ) throw (Exception){
  int result;
  msg = "PRIVMSG " + channel + " :" + msg + this->newline;
  result = bufferevent_write( this->buffevent, ( void* )( msg.c_str() ), msg.size() );

  if ( 0 != result ) {
    throw Exception( "bufferevent_write: failed" );
  }
}
