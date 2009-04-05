#ifndef LOGAPI_SERVICE_RPC_H
#define LOGAPI_SERVICE_RPC_H

#include <iostream>
#include <fstream>
#include <string>


#include "bb_rpc_controller.h"
#include "bb_rpc_service.h"

#include "logapi.pb.h"
#include "logapi_service.h"

namespace BeatBoard {
  class LogApiServiceRpc : public BeatBoard::BBRpcService {
  private:
    logapi::Response response;

  public:
    LogApiServiceRpc( logapi::RpcService* service);
    virtual ~LogApiServiceRpc();

    bool readRequest( const std::string& data );
    bool writeResult( std::string& data );
    void callServiceFunc( logapi::Request& request );
    void Done();
  };
}

#endif
