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

    // HTTP Header Dump
    if(evhttp_find_header (req->input_headers, "User-Agent")){
      evbuffer_add_printf( buf, evhttp_find_header (req->input_headers, "User-Agent"));
      evbuffer_add_printf( buf, "<br />\n" );
    }

    if(evhttp_find_header (req->input_headers, "Content-Length")){
      evbuffer_add_printf( buf, "Content-Length: " );
      evbuffer_add_printf( buf, evhttp_find_header (req->input_headers, "Content-Length"));
      evbuffer_add_printf( buf, "<br />\n" );
      printf("%s\n", req->input_buffer->buffer);
    }

    HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );
    map<string,string> get_parameters = instance->parseParameter(req->uri);

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


map<string, string> BeatBoard::HTTPAPIServer::parseParameter(char* uri){
  map<string, string> ret;

  struct evkeyvalq* params = (evkeyvalq *)calloc(1, sizeof(struct evkeyvalq));
  evhttp_parse_query(evhttp_decode_uri(uri), params);
  struct evkeyval *header;
  TAILQ_FOREACH(header, params, next) {
    ret[string(header->key)] = string(header->value);
  }
  free(params);
  return ret;
}

string create_simple_response(bool status, char* reason){
  string res = "{'status': ";
  res += (status ? string("'OK'") : string("'NG'")) + string(", ");
  res += "'reason' : '" + string(reason) + "'}";
  return res;
}

void BeatBoard::HTTPAPIServer::connectHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("CONNECT request");
  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }

  map<string, string> params = instance->parseParameter(req->uri);
  const string nick = params[string("nick")];
  const string server = params["server"];
  const string port = params["port"];
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
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("JOIN request");
  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }
  map<string, string> params = instance->parseParameter(req->uri);
  const string nick = params[string("nick")];
  const string channel = params["channel"];
  logger.debug("nick:" + nick + " channel:" + channel);  
  string res;
  try{
    IRCConnection *conn = instance->getIRCConnection( nick );
    conn->JOIN(channel);
    
    //evbuffer_add_printf( buf, "This is JOIN API" );
    //evhttp_send_reply( req, HTTP_OK, "OK", buf );
    res = create_simple_response(true, "joined");
    evbuffer_add_printf( buf,  res.c_str());
  } catch ( BeatBoard::Exception& error ) {
    logger.debug( error.message.data() );
    cerr << "irc coonection error\n";
    res = create_simple_response(true, "join failed");
    evbuffer_add_printf( buf,  res.c_str());
  }
  ostringstream csize;
  csize << res.size();
  evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());
  evhttp_send_reply( req, HTTP_OK, "OK", buf );
  evbuffer_free(buf);
}

void BeatBoard::HTTPAPIServer::speakHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("SPEAK request");

  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }

  map<string, string> params = instance->parseParameter(req->uri);
  const string nick = params[string("nick")];
  const string channel = params["channel"];
  const string message = params["message"];
  logger.debug("nick:" + nick + " channel:" + channel + " message:" + message);
  
  IRCConnection *conn = instance->getIRCConnection( nick );
  conn->PRIVMSG( channel, message );

  evbuffer_add_printf( buf, "This is SPEAK API" );
  evhttp_send_reply( req, HTTP_OK, "OK", buf );
  evbuffer_free(buf);
}

void BeatBoard::HTTPAPIServer::readHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("READ request");

  map<string, string> params = instance->parseParameter(req->uri);
  const string nick = params[string("nick")];
  logger.debug("nick:" + nick);

  IRCConnection *conn = NULL;
  struct evbuffer *buf;
  conn = instance->getIRCConnection( nick );
  buf = evbuffer_new();

  if(conn == NULL){
    evhttp_send_reply( req, HTTP_OK, "OK",  buf);
    return;
  }
  if(conn->hasMessage()){
    if ( buf == NULL ) {
      fprintf( stderr, "failed to create response buffer\n" );
      return;
    }
    HTTPAPINotifier* notifier =   new HTTPAPINotifier(req,  conn);
    notifier->notify((void*)NULL);
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
    delete(notifier);
  }else{
    HTTPAPINotifier* notifier =   new HTTPAPINotifier(req,  conn);
    conn->setNotifier(notifier);
  }
  evbuffer_free(buf);
}

BeatBoard::HTTPAPINotifier::HTTPAPINotifier(struct evhttp_request *req,  IRCConnection* conn){
  this->req = req;
  this->conn = conn;
}

BeatBoard::HTTPAPINotifier::~HTTPAPINotifier(){
}

void BeatBoard::HTTPAPINotifier::notify(void* arg){
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("NOTIFY");
  evhttp_request* req = this->req;
  this->req = NULL;
  IRCConnection* conn = this->conn;
  this->conn = NULL;

  struct evbuffer *buf;
  buf = evbuffer_new();
  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }
  //map<string,string> messages = conn->getMessage();
  map<string, vector<string> > messages = conn->getMessage();
  string resp = string("{");
  //map<string, string>::iterator it = messages.begin();
  map<string, vector<string> >::iterator it = messages.begin();
  while( it != messages.end() ){
    string key = "\"" + (*it).first + "\"";

    
    string val = "[";
    for( unsigned int i = 0; i < (*it).second.size(); i+=2){
      val += "\"" + (*it).second[i] + "\",";
      val += "\"" + (*it).second[i+1] + "\",";
    }
    val += "]";
    resp += key + ":" + val + ",";
    ++it;
  }
  resp += "}";

  evhttp_add_header(req->output_headers, "Content-type","application/x-javascript; charset=utf-8");

  ostringstream csize;
  csize << resp.size();
  evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());

  evbuffer_add( buf, resp.c_str(), resp.size() );
  evhttp_send_reply( req, HTTP_OK, "OK", buf );
  evbuffer_free(buf);
}
