#include "searchapi_service_rpc.h"

BeatBoard::SearchApiServiceRpc::SearchApiServiceRpc( searchapi::RpcService* service) :
  BBRpcService( service )
{
  std::cout << __func__ << " constractor" << std::endl;
}

BeatBoard::SearchApiServiceRpc::~SearchApiServiceRpc()
{
}

bool
BeatBoard::SearchApiServiceRpc::readRequest( const std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  searchapi::Request request;
  if ( !request.ParseFromString( data ) ) {
    std::cout << "Failed to parse request." << std::endl;
    return false;
  }

  callServiceFunc( request );

  return true;
}

bool
BeatBoard::SearchApiServiceRpc::writeResult( std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  if ( !response.SerializeToString( &data ) ) {
    std::cout << "Failed to parse response." << std::endl;
    return false;
  }

  return true;
}

void
BeatBoard::SearchApiServiceRpc::callServiceFunc( searchapi::Request& request )
{
  ((searchapi::RpcService*)service)->RpcFunc(new BeatBoard::BBRpcController(),
                                          &request, &response, 
                                          google::protobuf::NewCallback(this, &SearchApiServiceRpc::Done));
}

void
BeatBoard::SearchApiServiceRpc::Done()
{
  std::cerr << __func__ << std::endl;
}
