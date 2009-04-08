#ifndef LOGAPI_SERVICE_H
#define LOGAPI_SERVICE_H

#include <iostream>
#include <fstream>
#include <string>

#include "logapi.pb.h"
#include "logapi_service_common.h"
#include "drizzle_client.h"

namespace BeatBoard {
  class LogApiService : public logapi::RpcService {
  private:
	BeatBoard::DrizzleClient* client;

  public:
	LogApiService( const std::string& db, const std::string& tablename, 
                   const std::string& host, const in_port_t port );
	virtual ~LogApiService();
    void RpcFunc(google::protobuf::RpcController* controller,
                 const logapi::Request* request,
                 logapi::Response* response,
                 google::protobuf::Closure* done);
  };
}

#endif
