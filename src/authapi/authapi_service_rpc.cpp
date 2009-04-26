#include "authapi_service_rpc.h"

BeatBoard::AuthApiServiceRpc::AuthApiServiceRpc( authapi::RpcService* service) :
  BBRpcService( service )
{
  std::cout << __func__ << " constractor" << std::endl;
}

BeatBoard::AuthApiServiceRpc::~AuthApiServiceRpc()
{
}

bool
BeatBoard::AuthApiServiceRpc::readRequest( const std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  authapi::Request request;
  if ( !request.ParseFromString( data ) ) {
    std::cout << "Failed to parse request." << std::endl;
    return false;
  }

  callServiceFunc( request );

  return true;
}

bool
BeatBoard::AuthApiServiceRpc::writeResult( std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  if ( !response.SerializeToString( &data ) ) {
    std::cout << "Failed to parse response." << std::endl;
    return false;
  }

  return true;
}

void
BeatBoard::AuthApiServiceRpc::callServiceFunc( authapi::Request& request )
{
  ((authapi::RpcService*)service)->RpcFunc(new BeatBoard::BBRpcController(),
                                          &request, &response, 
                                          google::protobuf::NewCallback(this, &AuthApiServiceRpc::Done));
}

void
BeatBoard::AuthApiServiceRpc::Done()
{
  std::cerr << __func__ << std::endl;
}

