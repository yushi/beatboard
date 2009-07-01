#ifndef LOGAPI_SERVICE_H
#define LOGAPI_SERVICE_H

#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "logapi.pb.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "logapi_service_common.h"

#include "bb_protobuf_queue_memcached.h"


namespace BeatBoard {
  class LogApiServiceClientBbevqueue : public logapi::RpcService {
  private:
    google::protobuf::RpcChannel* channel;
    google::protobuf::RpcController* controller;
    logapi::RpcService* service;
    logapi::Request request;
    logapi::Response response;

    std::string rpcserver_host;
    int rpcserver_port;

    static void callback();
    bool dequeueLogData();
    
  public:
    LogApiServiceClientBbevqueue( const std::string& rpcserver_host,
                                  const int rpcserver_port );
    virtual ~LogApiServiceClientBbevqueue();
    void start();
  };
}

#endif
