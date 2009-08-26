#include "http_api_server.h"

using namespace std;

BeatBoard::HTTPAPIServer::HTTPAPIServer() {
  this->httpd = NULL;
  this->http_ev_base = NULL;
}

BeatBoard::HTTPAPIServer::~HTTPAPIServer() {
  if ( NULL != this->httpd ) {
    evhttp_free( httpd );
  }

  if ( NULL != this->http_ev_base ) {
    event_base_free( this->http_ev_base );
  }
}

void BeatBoard::HTTPAPIServer::setUp(char *addr, int port) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  this->http_ev_base = event_init();

  httpd = evhttp_new( this->http_ev_base );
  
  if ( evhttp_bind_socket( this->httpd, addr, port ) != 0 ) {};
  evhttp_set_cb( httpd, "/", HTTPAPIServer::rootHandler, NULL );

  evhttp_set_cb( httpd, "/CONNECT", HTTPAPIServer::connectHandler, this );
  evhttp_set_cb( httpd, "/JOIN", HTTPAPIServer::joinHandler, this );
  evhttp_set_cb( httpd, "/SPEAK", HTTPAPIServer::speakHandler, this );
  evhttp_set_cb( httpd, "/EXIT", HTTPAPIServer::exitHandler, this );
  evhttp_set_cb( httpd, "/READ", HTTPAPIServer::readHandler, this );
  logger.debug("setup finished");
}

void BeatBoard::HTTPAPIServer::rootHandler( struct evhttp_request *req, void *arg ) {
  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
  } else {
    evbuffer_add_printf( buf, "This is BeatBoard API Server<br />\n" );
    evbuffer_add_printf( buf, req->uri);
    evbuffer_add_printf( buf, "<br />\n" );

    // HTTPAPIServer Header Dump
    if(evhttp_find_header (req->input_headers, "User-Agent")){
      evbuffer_add_printf( buf, evhttp_find_header (req->input_headers, "User-Agent"));
      evbuffer_add_printf( buf, "<br />\n" );
    }

    if(EVHTTP_REQ_POST == req->type){
      evbuffer_add_printf( buf, "POST<br />\n" );
    }else if(EVHTTP_REQ_GET == req->type){
      evbuffer_add_printf( buf, "GET<br />\n" );
    }else if(EVHTTP_REQ_HEAD == req->type){
      evbuffer_add_printf( buf, "HEAD<br />\n" );
    }else{
      evbuffer_add_printf( buf, "Method is unknown<br />\n" );
    }
    HTTPAPIServer *instance = (HTTPAPIServer*)( arg );
    map<string,string> get_parameters = instance->parseParameter(req);
    //map<string,string> post_parameters = instance->parsePostParameter(req);
    string hogehoge = get_parameters["hogehoge"];
    if(hogehoge.size() != 0 ){
      evbuffer_add_printf( buf, (get_parameters["hogehoge"].c_str()));
      evbuffer_add_printf( buf, "<br />\n" );
      //POST???
      //      evbuffer_add_printf( buf, req->input_buffer);
      //evbuffer_add_printf( buf, "<br />\n" );
    }
    
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  }
  evbuffer_free(buf);
}
map<string, string> BeatBoard::HTTPAPIServer::parseParameter(struct evhttp_request *req){
  if(EVHTTP_REQ_POST == req->type){
    return this->parsePostParameter(req);
  }
  //  }else if(EVHTTP_REQ_GET == req->type){
  return this->parseGetParameter(req);
  //  }
}

map<string, string> BeatBoard::HTTPAPIServer::parsePostParameter(struct evhttp_request *req){
  map<string, string> ret;
  int content_length = atoi(evhttp_find_header (req->input_headers, "Content-Length"));
  struct evkeyvalq* params = (evkeyvalq *)calloc(1, sizeof(struct evkeyvalq));
  string dummy_url = string("http://localhost/?");
  char *body = (char*)calloc(1,sizeof(char) * content_length + 1);
  evbuffer_remove(req->input_buffer, (void*)body, content_length);

  //replace "+" to " " for www-form-urlencoded
  int i = 0;
  for(i = 0; i < content_length; i++){
    if( *(body+i) == '+'){
      *(body+i) = ' ';
    }
  }
  dummy_url += string(body);

  evhttp_parse_query(dummy_url.c_str(), params);
  struct evkeyval *header = (evkeyval*)calloc(1, sizeof(struct evkeyval));
  TAILQ_FOREACH(header, params, next) {
    ret[string(header->key)] = string(header->value);
  }

  free(params);
  free(body);
  free(header);
  return ret;
}

map<string, string> BeatBoard::HTTPAPIServer::parseGetParameter(struct evhttp_request *req){
  map<string, string> ret;
  const char* uri = req->uri;
  struct evkeyvalq* params = (evkeyvalq *)calloc(1, sizeof(struct evkeyvalq));
  evhttp_parse_query(evhttp_decode_uri(uri), params);
  struct evkeyval *header = (evkeyval*)calloc(1, sizeof(struct evkeyval));
  TAILQ_FOREACH(header, params, next) {
    ret[string(header->key)] = string(header->value);
  }
  free(params);
  free(header);
  return ret;
}

string create_simple_response(bool status, const char* reason){
  string res = "{'status': ";
  res += (status ? string("'OK'") : string("'NG'")) + string(", ");
  res += "'reason' : '" + string(reason) + "'}";
  return res;
}

void BeatBoard::HTTPAPIServer::connectHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = (HTTPAPIServer*)( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("CONNECT request");
  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }

  map<string, string> params = instance->parseParameter(req);
  const string nick = params[string("nick")];
  const string server = params[string("server")];
  const string port = params[string("port")];
  logger.debug("nick:" + nick + " server:" + server + " port:" + port);
  
  IRCConnection *conn = NULL;
  conn = instance->getIRCConnection( nick );
  string res;
  try{
    if(NULL == conn){
      IRCConnection *newConnection = new IRCConnection(nick);
      instance->setIRCConnection( nick, newConnection );
      newConnection->connectIRCServer(server, port);
      res = create_simple_response(true, "connection created");
      evbuffer_add_printf( buf,  res.c_str());
    }else{
      map<string, IRCChannel>::iterator it = (conn->received).begin();
      res += "{'status': 'OK', 'users': {";
      while(it != (conn->received).end()){
        res +=  "'" + it->first + "': [";
        vector<string> users = (it->second).getUsers();
        vector<string>::iterator user = users.begin();
        while(user != users.end()){
          res += "'" + *user + "'";
          ++user;
        }
        res += "]}";
        ++it;
      }
      res += "}";
      res = create_simple_response(true, "connection found");
      evbuffer_add_printf( buf,  res.c_str());
    }
    ostringstream csize;
    csize << res.size();
    evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());
    evhttp_send_reply( req, HTTP_OK, "OK", buf );

  }catch ( BeatBoard::Exception& error ) {
    logger.debug( error.message.data() );
    cerr << "irc coonection error\n";
    res = create_simple_response(true, "connection create failed");
    evbuffer_add_printf( buf,  res.c_str());
    ostringstream csize;
    csize << res.size();
    evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  }
  evbuffer_free(buf);  
  return;
}

void BeatBoard::HTTPAPIServer::exitHandler( struct evhttp_request *req, void *arg ) {
  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }
  
  try{
    evbuffer_add_printf( buf, "This is EXIT API" );
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  } catch ( BeatBoard::Exception& error ) {
    BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
    logger.debug( error.message.data() );
    cerr << "irc coonection error\n";
  }
  evbuffer_free(buf);
  exit(0);
}

void BeatBoard::HTTPAPIServer::joinHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = (HTTPAPIServer*)( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("JOIN request");
  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }
  map<string, string> params = instance->parseParameter(req);
  const string nick = params[string("nick")];
  const string channel = params[string("channel")];
  logger.debug("nick:" + nick + " channel:" + channel);  
  string res;
  try{
    IRCConnection *conn = instance->getIRCConnection( nick );
    if(conn == NULL){
      BeatBoard::Exception notfound(string("connection not found"));
      throw notfound;
    }
    conn->JOIN(channel);
    
    //evbuffer_add_printf( buf, "This is JOIN API" );
    //evhttp_send_reply( req, HTTP_OK, "OK", buf );
    res = create_simple_response(true, "joined");
  } catch ( BeatBoard::Exception& error ) {
    logger.debug( error.message.data() );
    cerr << "irc coonection error\n";
    res = create_simple_response(true, "join failed");
  }
  evbuffer_add_printf( buf,  res.c_str());
  ostringstream csize;
  csize << res.size();
  evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());
  evhttp_send_reply( req, HTTP_OK, "OK", buf );
  evbuffer_free(buf);
}

void BeatBoard::HTTPAPIServer::speakHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = (HTTPAPIServer*)( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("SPEAK request");

  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }

  map<string, string> params = instance->parseParameter(req);
  const string nick = params[string("nick")];
  const string channel = params[string("channel")];
  const string message = params[string("message")];
  logger.debug("nick:" + nick + " channel:" + channel + " message:" + message);
  

  IRCConnection *conn = instance->getIRCConnection( nick );
  if(conn == NULL){
    evbuffer_add_printf( buf, "connection not found" );
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  }else{
    conn->PRIVMSG( channel, message );
    evbuffer_add_printf( buf, "This is SPEAK API" );
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  }
  evbuffer_free(buf);
}

void BeatBoard::HTTPAPIServer::readHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = (HTTPAPIServer*)( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("READ request");

  map<string, string> params = instance->parseParameter(req);
  const string nick = params[string("nickname")];
  logger.debug("nick:" + nick);

  IRCConnection *conn = NULL;
  struct evbuffer *buf;
  conn = instance->getIRCConnection( nick );
  buf = evbuffer_new();

  if(conn == NULL){
    logger.debug("connection not found");
    evhttp_send_reply( req, HTTP_OK, "OK",  buf);
    return;
  }
  if(conn->hasMessage()){
    if ( buf == NULL ) {
      fprintf( stderr, "failed to create response buffer\n" );
      return;
    }
    HTTPAPIReadNotifier* notifier =   new HTTPAPIReadNotifier(req);
    conn->setReadNotifier(notifier);
    conn->notifyRead();
  }else{
    logger.debug("message not found");
    HTTPAPIReadNotifier* notifier =   new HTTPAPIReadNotifier(req);
    conn->setReadNotifier(notifier);
  }
  evbuffer_free(buf);
}

