#include <sys/queue.h>
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
  this->http_ev_base = event_init();

  httpd = evhttp_new( this->http_ev_base );
  
  if ( evhttp_bind_socket( this->httpd, addr, port ) != 0 ) {};
  evhttp_set_cb( httpd, "/", HTTPAPIServer::rootHandler, NULL );

  evhttp_set_cb( httpd, "/JOIN", HTTPAPIServer::joinHandler, this );
  evhttp_set_cb( httpd, "/SPEAK", HTTPAPIServer::speakHandler, this );
  evhttp_set_cb( httpd, "/READ", HTTPAPIServer::readHandler, this );
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

}


map<string, string> BeatBoard::HTTPAPIServer::parseParameter(char* uri){
  map<string, string> ret;

  struct evkeyvalq* params = (evkeyvalq *)calloc(1, sizeof(struct evkeyvalq));
  fprintf(stderr,"encoded uri: %s\n", uri);
  fprintf(stderr,"decoded uri: %s\n", evhttp_decode_uri(uri));
  evhttp_parse_query(evhttp_decode_uri(uri), params);

  struct evkeyval *header;
  TAILQ_FOREACH(header, params, next) {
    ret[string(header->key)] = string(header->value);
    fprintf(stderr,"KEY: %s, VAL: %s\n", header->key, header->value );
  }
  free(params);
  return ret;
}


void BeatBoard::HTTPAPIServer::joinHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );

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
  const string channel = params["channel"];
  
  try{
    IRCConnection *newConnection = new IRCConnection(nick);
    instance->setIRCConnection( nick, newConnection );
    newConnection->connectIRCServer(server, port);
    newConnection->JOIN(channel);
    
    evbuffer_add_printf( buf, "This is JOIN API" );
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  } catch ( BeatBoard::Exception& error ) {
    BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
    logger.debug( error.message.data() );
    cerr << "irc coonection error\n";
  }

}

void BeatBoard::HTTPAPIServer::speakHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );

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

  IRCConnection *conn = instance->getIRCConnection( nick );
  conn->PRIVMSG( channel, message );

  evbuffer_add_printf( buf, "This is SPEAK API" );
  evhttp_send_reply( req, HTTP_OK, "OK", buf );

}

void BeatBoard::HTTPAPIServer::readHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );



  map<string, string> params = instance->parseParameter(req->uri);
  const string nick = params[string("nick")];

  IRCConnection *conn = instance->getIRCConnection( nick );
  if(conn->hasMessage()){
    struct evbuffer *buf;
    buf = evbuffer_new();
    if ( buf == NULL ) {
      fprintf( stderr, "failed to create response buffer\n" );
      return;
    }
    cout << "in not comet" << endl;
    evbuffer_add_printf( buf, "This is READ API" );
    evbuffer_add_printf( buf, (conn->received["#yushi"]).c_str() );
    conn->received["#yushi"] = "";
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  }else{
    HTTPAPINotifier* notifier =   new HTTPAPINotifier(req,  conn);
    conn->setNotifier(notifier);
  }
}

BeatBoard::HTTPAPINotifier::HTTPAPINotifier(struct evhttp_request *req,  IRCConnection* conn){
  this->req = req;
  this->conn = conn;
}

BeatBoard::HTTPAPINotifier::~HTTPAPINotifier(){
}

void BeatBoard::HTTPAPINotifier::notify(void* arg){
  struct evbuffer *buf;
  buf = evbuffer_new();
  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return;
  }
  cout << "in comet" << endl;
  evbuffer_add_printf( buf, "This is READ API" );
  evbuffer_add_printf( buf, (this->conn->received["#yushi"]).c_str() );
  conn->received["#yushi"] = "";
  evhttp_send_reply( this->req, HTTP_OK, "OK", buf );
  this->req = NULL;
  this->conn = NULL;
}
