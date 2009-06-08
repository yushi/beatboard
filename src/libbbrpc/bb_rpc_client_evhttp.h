#ifndef BB_RPC_CLIENT_EVHTTP_H
#define BB_RPC_CLIENT_EVHTTP_H

#include <event.h>
#include <evhttp.h>
#include <iostream>
#include <string>
#include <cstdlib>

class BBRpcClientEvhttp {
private:
  struct evhttp *httpd;
  struct event_base *http_ev_base;
  std::string host;
  int port;

  static void rootHandler( struct evhttp_request *req, void *arg );

public:
  BBRpcClientEvhttp( const std::string& host, const int port );
  ~BBRpcClientEvhttp();
  bool start( const std::string& uri,
              void (*cb)(struct evhttp_request *, void *),
              void *arg);
  static bool find_header( const struct evkeyvalq* headers,
						   const std::string& key,
						   std::string& val );
  static std::string uniq_id();
};

#endif
