#ifndef LOGAPI_SERVICE_H
#define LOGAPI_SERVICE_H

#include <uuid/uuid.h>

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "logapi.pb.h"
#include "logapi_service_common.h"
#include "drizzle_client.h"

namespace BeatBoard {
  class LogApiService : public logapi::RpcService {
  private:
	BeatBoard::DrizzleClient* client;
    BeatBoard::DrizzleResponse response;
    std::string table_name;

    bool insertLogToDB( std::string& channel, std::string& time, std::string& identifier, std::string& message );
    void replaceEscapeChar( std::string& data, std::string& escapechar);
    std::string escape( const std::string& data );

  public:
	LogApiService( const std::string& db, const std::string& table_name, 
                   const std::string& host, const in_port_t port );
	virtual ~LogApiService();
    void RpcFunc( google::protobuf::RpcController* controller,
                  const logapi::Request* request,
                  logapi::Response* response,
                  google::protobuf::Closure* done );
  };
}

#endif
