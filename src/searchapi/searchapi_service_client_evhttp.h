#ifndef SEARCHAPI_SERVICE_CLIENT_EVHTTP_H
#define SEARCHAPI_SERVICE_CLIENT_EVHTTP_H

#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>
#include <event.h>
#include <evhttp.h>

#include "searchapi.pb.h"
#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "bb_rpc_client_evhttp.h"

#include "searchapi_service_common.h"

class SearchapiServiceClientEvhttp {
private:
  google::protobuf::RpcChannel* channel;
  google::protobuf::RpcController* controller;
  searchapi::RpcService* service;
  searchapi::Request request;
  searchapi::Response response;

  std::string uri;
  std::string evhttp_host;
  int evhttp_port;
  std::string rpcserver_host;
  int rpcserver_port;

  static void callback();
  void sendQuery( const std::string query );
  std::string search( char *query );
  static void searchHandler( struct evhttp_request *req, void *arg );

public:
  SearchapiServiceClientEvhttp( const std::string& evhttp_host,
                                const int evhttp_port,
                                const std::string& rpcserver_host,
                                const int rpcserver_port );
  ~SearchapiServiceClientEvhttp();
  void start();
};

#endif
