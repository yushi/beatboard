#ifndef SAMPLE_RPC_CLIENT_EVHTTP_H
#define SAMPLE_RPC_CLIENT_EVHTTP_H

#include <event.h>
#include <evhttp.h>
#include <iostream>
#include <string>

class SampleRpcClientEvhttp {
private:
  struct evhttp *httpd;
  struct event_base *http_ev_base;
  std::string host;
  int port;

  static void rootHandler( struct evhttp_request *req, void *arg );

public:
  SampleRpcClientEvhttp( const std::string& host, const int port );
  ~SampleRpcClientEvhttp();
  bool start( const std::string& uri,
              void (*cb)(struct evhttp_request *, void *) );
};

#endif
