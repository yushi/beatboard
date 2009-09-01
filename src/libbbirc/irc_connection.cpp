#include "irc_connection.h"
//FIXME 文字列操作を連結でやるのは汚い
struct event_base *ev_base = NULL;

// static field
const string BeatBoard::IRCConnection::newline = string("\r\n");
const string BeatBoard::IRCConnection::RPL_NAMREPLY = string("353");
const string BeatBoard::IRCConnection::RPL_ENDOFNAMES = string("366");

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
  while(str_stream.getline(buf, 1024)){
    printf("readed: %s\n", buf);
    BeatBoard::IRCEvent *event = BeatBoard::parse_irc_message(buf);
    if(event != NULL){
      if(*(event->command) == string("PING")){
        irc_conn->PONG( *(event->params[0]) );
      }else if(*(event->command) == string("PRIVMSG")){
        string channel = *(event->params[0]);
        string message = *(event->params[1]);
        string prefix = *(event->prefix);

        irc_conn->received[channel].addMessage(prefix, message);
        
        irc_conn->notifyRead();
        irc_conn->loggingMessage(channel, prefix, message);
        
      }else if(*(event->command) == BeatBoard::IRCConnection::RPL_NAMREPLY){
        string users = *(event->params[3]);
        size_t pos;
        while( (pos = users.find(' ')) != string::npos){
          irc_conn->received[*(event->params[2])].addUser(users.substr(0, pos));
          users = users.substr(pos + 1, users.size() - 1);
        }
        irc_conn->received[*(event->params[2])].addUser(users);
      }else if(*(event->command) == BeatBoard::IRCConnection::RPL_ENDOFNAMES){
        irc_conn->received[*(event->params[1])].addUserEnd();
        irc_conn->notifyJoin();
      }else if(*(event->command) == string("JOIN")){
        string channel = *(event->params[0]);
        string message = *(event->command);
        string prefix = *(event->prefix);

        irc_conn->received[channel].addMessage(message, prefix);
        
        irc_conn->notifyRead();
        irc_conn->loggingMessage(channel, prefix, message);
        irc_conn->received[channel].addUserJoin(prefix);
        
      }else if(*(event->command) == string("PART")){
        string channel = *(event->params[0]);
        string message = *(event->command) + string(" ") + *(event->params[1]);
        string prefix = *(event->prefix);
        irc_conn->received[channel].addMessage(message, prefix);
        irc_conn->received[channel].delUser(prefix);
        
        irc_conn->notifyRead();
        irc_conn->loggingMessage(channel, prefix, message);
        
      }else if(*(event->command) == string("QUIT")){
        string message = *(event->command) + string(" ") + *(event->params[0]);
        string prefix = *(event->prefix);

        map<string,BeatBoard::IRCChannel>::iterator it =
          (irc_conn->received).begin();
        while(it != (irc_conn->received).end()){
          irc_conn->received[it->first].addMessage(message, prefix);
          irc_conn->received[it->first].delUser(prefix);
          irc_conn->loggingMessage(it->first, prefix, message);
          ++it;
        }

        irc_conn->notifyRead();
      }
      delete event;
    }else{

      //event not found
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
  //hints.ai_family = AF_UNSPEC;
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;
  
  result = getaddrinfo(addr.c_str(),port.c_str(),&hints,&addrinfo);
  if ( 0 != result  ) {
    throw Exception( "error: invalid address" );
  }

  if ( 0 != connect( this->sock, addrinfo->ai_addr, addrinfo->ai_addrlen ) ) {
    // TODO error handling, addrinfo free
    throw Exception( "connect: failed" );
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
  this->loggingMessage(channel, this->nick, text);
  this->write(message);
}

void BeatBoard::IRCConnection::loggingMessage( string channel, string identifier, string message ){
  stringstream unixtime;
  unixtime << (int)time(NULL);

  BeatBoard::ProtobufQueueMemcached queue;
  logapi::Request request;
  request.set_channel(channel.c_str());
  request.set_time(unixtime.str().c_str());
  request.set_identifier(identifier.c_str());
  request.set_message(message.c_str());
  std::string data;
  if ( !request.SerializeToString( &data ) ){
    std::cout << "Failed to parse request." << std::endl;
    //error...
    //return false;
  }
  
  int count = 3;
  for (int i = 0; i < count; i++){
    if (queue.enqueue(data) != -1)
      {
        break;
      }
  }
}

void BeatBoard::IRCConnection::setReadNotifier(BeatBoard::Notifier* notifier){
  this->readNotifier.push_back(notifier);
}
void BeatBoard::IRCConnection::setJoinNotifier(BeatBoard::Notifier* notifier){
  this->joinNotifier.push_back(notifier);
}

bool BeatBoard::IRCConnection::notify(map<string, vector<string> > messages,
                                      vector<Notifier*>* notifiers){
  vector<Notifier*>::iterator it = notifiers->begin();
  bool notify_success = false;
  while(it != notifiers->end()){
    bool result = (*it)->notify(&messages);
    delete((*it));
    
    if(result){
      notify_success = true;
    }
    
    ++it;
  }
  notifiers->clear();
  return notify_success;
}

void BeatBoard::IRCConnection::notifyJoin(){
  map<string, vector<string> > messages;
  map<string, IRCChannel >::iterator it = (this->received).begin();
  while(it != this->received.end()){
    messages[it->first] = it->second.getUsers();
    ++it;
  }

    /*
    res = create_simple_response(true,
                                 users_str.c_str());
    cout << "USERS: "<< users_str << endl;
    evbuffer_add_printf( buf,  res.c_str());
    */

  /*
    if(!(this->notify(messages, this->joinNotifier))){
    map<string, IRCChannel>::iterator it = this->received.begin();
    while( it != this->received.end() ){
      (*it).second.recoverMessages();
      ++it;
    }
  }
  */

}

void BeatBoard::IRCConnection::notifyRead(){
  map<string, vector<string> > messages = this->getMessage();

  if(!(this->notify(messages, &this->readNotifier))){
    map<string, IRCChannel>::iterator it = this->received.begin();
    while( it != this->received.end() ){
      (*it).second.recoverMessages();
      ++it;
    }
  }

  return;
}

BeatBoard::IRCConnection::~IRCConnection(){
  close(this->sock);
  if(this->buffevent){
    bufferevent_disable(this->buffevent, EV_READ | EV_WRITE);
    bufferevent_free(this->buffevent);
  }
}

bool BeatBoard::IRCConnection::hasMessage(){
  map<string, IRCChannel  >::iterator it = this->received.begin();
  while( it != this->received.end() ){
    if((*it).second.hasMessage()){
      return true;
    }
    ++it;
  }
  return false;
}

map<string, vector<string> > BeatBoard::IRCConnection::getMessage(){
  map<string, vector<string> > ret;
  map<string, IRCChannel>::iterator it = this->received.begin();
  while( it != this->received.end() ){
    ret[(*it).first] = (*it).second.getMessages();
    ++it;
  }
  return ret;
}


BeatBoard::Notifier::~Notifier(){
}

bool BeatBoard::Notifier::notify(map<string, vector<string> >*){
  return false;
}
