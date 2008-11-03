#include "http_api_server.h"

BeatBoard::HTTPAPIServer::HTTPAPIServer() {
  this->httpd = NULL;
  this->ev_base = NULL;
}

BeatBoard::HTTPAPIServer::~HTTPAPIServer() {
  if ( NULL != this->httpd ) {
    evhttp_free( httpd );
  }

  if ( NULL != this->ev_base ) {
    event_base_free( this->ev_base );
  }
}

void BeatBoard::HTTPAPIServer::setUp(int port) {
  this->ev_base = event_init();

  httpd = evhttp_new( this->ev_base );

  if ( evhttp_bind_socket( this->httpd, "0.0.0.0", port ) != 0 ) {}

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
    evbuffer_add_printf( buf, "This is BeatBoard API Server" );
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
      instance->setIRCConnection( newConnection );
      newConnection->connectIRCServer(std::string("localhost"), 6667);
      newConnection->joinIRCChannel("#beatboard");
      
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
    IRCConnection *a = instance->getIRCConnection( std::string( "yushi_bbircd" ) );
    a->privMSG( std::string("#beatboard"), std::string( "hogera-" ) );

    evbuffer_add_printf( buf, "This is SPEAK API" );
    evhttp_send_reply( req, HTTP_OK, "OK", buf );
    }
}
