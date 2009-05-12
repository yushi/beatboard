#include "http_api_server.h"

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

    // ここで input_headers が GET引数に変わる
    // evkeyvalq 型が確保できないので input_headersを使う
    evhttp_parse_query(evhttp_decode_uri(req->uri), req->input_headers);


    const char *hogehoge = evhttp_find_header (req->input_headers, "hogehoge");
    if(hogehoge != NULL){
      evbuffer_add_printf( buf, evhttp_find_header (req->input_headers, "hogehoge"));
      evbuffer_add_printf( buf, "<br />\n" );
      //      evbuffer_add_printf( buf, req->input_buffer);
      //evbuffer_add_printf( buf, "<br />\n" );
        }
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
  }

}

void BeatBoard::HTTPAPIServer::joinHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );

  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
  } else {
    try{
      IRCConnection *newConnection = new IRCConnection(std::string("yushi_new"));
      instance->setIRCConnection( std::string("yushi_new"), newConnection );
      //FIXME server and port fixed
      //newConnection->connectIRCServer(std::string("localhost"), "6667");
      newConnection->connectIRCServer(std::string("irc.freenode.net"), "6667");
      newConnection->JOIN("#yushi");
      
      evbuffer_add_printf( buf, "This is JOIN API" );
      
      evhttp_send_reply( req, HTTP_OK, "OK", buf );

    } catch ( BeatBoard::Exception& error ) {
      BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
      logger.debug( error.message.data() );
      std::cerr << "irc coonection error\n";
    }
  }
}

void BeatBoard::HTTPAPIServer::speakHandler( struct evhttp_request *req, void *arg ) {
  HTTPAPIServer *instance = static_cast<HTTPAPIServer*>( arg );

  struct evbuffer *buf;
  buf = evbuffer_new();

  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
  } else {
    IRCConnection *a = instance->getIRCConnection( std::string( "yushi_new" ) );
    a->PRIVMSG( std::string("#yushi"), std::string( "hogera-" ) );

    evbuffer_add_printf( buf, "This is SPEAK API" );
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
    }
}
