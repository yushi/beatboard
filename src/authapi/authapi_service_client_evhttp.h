#ifndef AUTHAPI_SERVICE_CLIENT_EVHTTP_H
#define AUTHAPI_SERVICE_CLIENT_EVHTTP_H

#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <event.h>
#include <string>
#include <map>
#include <ctime>

#include "authapi.pb.h"
#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "bb_rpc_client_evhttp.h"

#include "authapi_service_common.h"

class AuthapiServiceClientEvhttp {
private:
  google::protobuf::RpcChannel* channel;
  google::protobuf::RpcController* controller;
  authapi::RpcService* service;
  authapi::Request request;
  authapi::Response response;

  std::string uri;
  std::string evhttp_host;
  int evhttp_port;
  std::string rpcserver_host;
  int rpcserver_port;

  static void callback();

  unsigned int updateUserInfo( std::map<std::string, std::string> params );
  unsigned int verifyUser( std::map<std::string, std::string> params );
  unsigned int addUser( std::map<std::string, std::string> params );
  std::map<std::string, std::string> parseHeaders( struct evkeyvalq& headers );
  static void authHandler( struct evhttp_request *req, void *arg );

  void setUpRpc();
  void deleteRpc();
 
public:
  AuthapiServiceClientEvhttp( const std::string& evhttp_host,
                              const int evhttp_port,
                              const std::string& rpcserver_host,
                              const int rpcserver_port );
  ~AuthapiServiceClientEvhttp();
  void start();
};

#endif
