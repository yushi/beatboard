#include "logapi_service_rpc.h"

BeatBoard::LogApiServiceRpc::LogApiServiceRpc( logapi::RpcService* service) :
  BBRpcService( service )
{
  std::cout << __func__ << " constractor" << std::endl;
}

BeatBoard::LogApiServiceRpc::~LogApiServiceRpc()
{
}

bool
BeatBoard::LogApiServiceRpc::readRequest( const std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  logapi::Request request;
  if ( !request.ParseFromString( data ) ) {
    std::cout << "Failed to parse request." << std::endl;
    return false;
  }
  std::cout << "client query: " << request.query() << std::endl;
  callServiceFunc( request );

  return true;
}

bool
BeatBoard::LogApiServiceRpc::writeResult( std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  if ( !response.SerializeToString( &data ) ) {
    std::cout << "Failed to parse response." << std::endl;
    return false;
  }

  return true;
}

void
BeatBoard::LogApiServiceRpc::callServiceFunc( logapi::Request& request )
{
  ((logapi::RpcService*)service)->RpcFunc(new BeatBoard::BBRpcController(),
                                          &request, &response, 
                                          google::protobuf::NewCallback(this, &LogApiServiceRpc::Done));
}

void
BeatBoard::LogApiServiceRpc::Done()
{
  std::cerr << __func__ << std::endl;
}

