#ifndef AUTHAPI_SERVICE_RPC_H
#define AUTHAPI_SERVICE_RPC_H

#include <iostream>
#include <fstream>
#include <string>


#include "bb_rpc_controller.h"
#include "bb_rpc_service.h"

#include "authapi.pb.h"
#include "authapi_service.h"

namespace BeatBoard {
  class AuthApiServiceRpc : public BeatBoard::BBRpcService {
  private:
    authapi::Response response;

  public:
    AuthApiServiceRpc( authapi::RpcService* service);
    virtual ~AuthApiServiceRpc();

    bool readRequest( const std::string& data );
    bool writeResult( std::string& data );
    void callServiceFunc( authapi::Request& request );
    void Done();
  };
}

#endif
