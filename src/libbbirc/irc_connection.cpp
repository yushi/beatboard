#include "irc_connection.h"
//FIXME 文字列操作を連結でやるのは汚い
struct event_base *ev_base = NULL;

// static field
string BeatBoard::IRCConnection::newline = string("\r\n");

bool BeatBoard::IRCConnection::bb_event_dispatch(struct event_base *ev){
  if(ev != NULL){
    ev_base = ev;
  }
  if(NULL == ev_base){
    ev_base = event_init();
  }
  //int ret = event_base_loop(ev_base,0);
  int ret = event_dispatch();
  if( -1 == ret || 1 == ret){
    //TODO error handling
    return false;
  }
  
  return true;
}

void BeatBoard::IRCConnection::bb_event_finish(){
  if(ev_base != NULL){
    struct timeval time;
    time.tv_sec = 0;
    time.tv_usec = 0;
    event_base_loopexit	(ev_base,&time);
    event_loopexit(&time);
  }
}

/* readable event handler for libevent */

void irc_buffevent_read( struct bufferevent *bev, void *arg ) {
  BeatBoard::IRCConnection *irc_conn = (BeatBoard::IRCConnection*)arg;
  char buf[1024];
  stringstream str_stream;
  size_t read_size = 0;
  while ( 0 < (read_size = bufferevent_read( bev, buf, 1023 ) ) ){
    buf[read_size] = 0;
    str_stream << string(buf);
  }
  //debug
  //cout << str_stream.str() << endl;
  while(str_stream.getline(buf, 1024)){
    BeatBoard::IRCEvent *hoge = BeatBoard::parse_irc_message(buf);
    if(hoge != NULL){
      if(*(hoge->command) == string("PING")){
        irc_conn->PONG( *(hoge->params[0]) );
      }else if(*(hoge->command) == string("PRIVMSG")){
        string channel = *(hoge->params[0]);
        string message = *(hoge->params[1]);
        irc_conn->received[channel] += message;
        BeatBoard::Notifier* notifier = irc_conn->getNotifier();
        if( notifier != NULL){
          notifier->notify((void*)NULL);
          cout << "notifier is alive" << endl;
          irc_conn->setNotifier(NULL);
        }else{
          cout << "notifier is dead" << endl;
        }

      }
      delete hoge;
    }else{
      cout << "event not found" << endl;
    }
  }
}

/* writable event handler for libevent */

void irc_buffevent_write( struct bufferevent *bev, void *arg ) {
}

/* error event handler for libevent */

void irc_buffevent_error( struct bufferevent *bev, short what, void *arg ) {
  //BeatBoard::IRCConnection *irc_conn = (BeatBoard::IRCConnection*)arg;
  //FIXME cleanup bufferevent
}


BeatBoard::IRCConnection::IRCConnection(string nick) {
  this->nick = nick;
  this->notifier = NULL;
  if(NULL == ev_base){
    ev_base = event_init();
  }
}

void BeatBoard::IRCConnection::create_socket() throw( Exception ) {
  this->sock = socket( AF_INET, SOCK_STREAM, 0 );

  if ( this->sock == -1 ) {
    string message = string( "socket: " );
    message += strerror( errno );
    throw Exception( message );
  }
}

void BeatBoard::IRCConnection::connectIRCServer(string addr, string port) throw (Exception){
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
    // TODO error handling
    printf( "connect failed\n" );
    exit( -1 );
  }
  freeaddrinfo(addrinfo);

  /* add event to this connection */
  this->buffevent = bufferevent_new( this->sock,
                                     irc_buffevent_read,
                                     irc_buffevent_write,
                                     irc_buffevent_error,
                                     (void*)this );
  if ( NULL == this->buffevent ) {
    throw Exception( "bufferevent_new: failed" );
  }

  result = bufferevent_enable( this->buffevent, EV_READ | EV_WRITE );

  if ( 0 != result ) {
    throw Exception( "bufferevent_enable: failed" );
  }

  this->NICK(this->nick);
  this->USER(this->nick, "0", "*", ":beatboard");
}

void BeatBoard::IRCConnection::disconnectIRCServer(void) throw (Exception){
  if(this->sock){
    close(this->sock);
  }
}
  
void BeatBoard::IRCConnection::write(string str) throw (Exception){
  int result;
  str += this->newline;
  result =   bufferevent_write( this->buffevent, str.c_str(), str.length());
  if ( 0 != result ) {
    throw Exception( "bufferevent_write: failed" );
  }
  //debug
  cout << str << endl;
}

void BeatBoard::IRCConnection::NICK(string name) throw (Exception){
  string message("NICK :" + name);
  this->write(message);
}

void BeatBoard::IRCConnection::USER(string user, string host, string server, string real) throw (Exception){
  string message("USER " + user + " " + host + " " + server + " :" + real);
  this->write(message);
}

void BeatBoard::IRCConnection::PONG(string server) throw (Exception){
  string message("PONG :" + server);
  this->write(message);
}

void BeatBoard::IRCConnection::JOIN(string channel) throw (Exception){
  string message("JOIN :" + channel);
  this->write(message);
}


void BeatBoard::IRCConnection::PRIVMSG( string channel, string text ) throw (Exception){
  string message("PRIVMSG " + channel + " :" + text);
  this->write(message);
}

BeatBoard::Notifier* BeatBoard::IRCConnection::getNotifier(){
  return this->notifier;
}

void BeatBoard::IRCConnection::setNotifier(BeatBoard::Notifier* notifier){
  this->notifier = notifier;
}

BeatBoard::IRCConnection::~IRCConnection(){
  close(this->sock);
  if(this->buffevent){
    bufferevent_disable(this->buffevent, EV_READ | EV_WRITE);
    bufferevent_free(this->buffevent);
  }
}

bool BeatBoard::IRCConnection::hasMessage(){
  map<string, string>::iterator it = this->received.begin();
  while( it != this->received.end() ){
    //cout << (*it).first << ":" << (*it).second << endl;
    if((*it).second.size() != 0){
      return true;
    }
    ++it;
  }
  return false;
}

BeatBoard::Notifier::~Notifier(){
  cout << "Thisis parent dest" << endl;
}

void BeatBoard::Notifier::notify(void *){
  cout << "Thisis parent notify" << endl;
}
