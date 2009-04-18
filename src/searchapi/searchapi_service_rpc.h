#ifndef SEARCHAPI_SERVICE_RPC_H
#define SEARCHAPI_SERVICE_RPC_H

#include <iostream>
#include <fstream>
#include <string>


#include "bb_rpc_controller.h"
#include "bb_rpc_service.h"

#include "searchapi.pb.h"
#include "searchapi_service.h"

namespace BeatBoard {
  class SearchApiServiceRpc : public BeatBoard::BBRpcService {
  private:
    searchapi::Response response;

  public:
    SearchApiServiceRpc( searchapi::RpcService* service);
    virtual ~SearchApiServiceRpc();

    bool readRequest( const std::string& data );
    bool writeResult( std::string& data );
    void callServiceFunc( searchapi::Request& request );
    void Done();
  };
}

#endif
