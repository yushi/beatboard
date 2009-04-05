#ifndef LOGAPI_SERVICE_H
#define LOGAPI_SERVICE_H

#include <iostream>
#include <fstream>
#include <string>

#include "logapi.pb.h"

namespace BeatBoard {
  class LogApiService : public logapi::RpcService {
  public:
    void RpcFunc(google::protobuf::RpcController* controller,
                 const logapi::Request* request,
                 logapi::Response* response,
                 google::protobuf::Closure* done);
  };
}

#endif
