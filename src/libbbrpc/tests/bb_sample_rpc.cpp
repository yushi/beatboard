#include "bb_sample_rpc.h"

BBSampleRpc::BBSampleRpc( sample::RpcService* service ) : BBRpcService( service )
{
  std::cout << __func__ << " constractor" << std::endl;
}

bool
BBSampleRpc::readRequest( const std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  sample::Request request;
  if ( !request.ParseFromString( data ) ) {
    std::cout << "Failed to parse request." << std::endl;
    return false;
  }
  std::cout << "client query: " << request.query() << std::endl;
  callServiceFunc( request );

  return true;
}

bool
BBSampleRpc::writeResult( std::string& data )
{
  std::cout << __func__ << " called" << std::endl;

  if ( !response.SerializeToString( &data ) ) {
    std::cout << "Failed to parse response." << std::endl;
    return false;
  }

  return true;
}

void
BBSampleRpc::callServiceFunc( sample::Request& request )
{
  ((sample::RpcService*)service)->RpcFunc(new BeatBoard::BBRpcController(),
                                          &request, &response, 
                                          google::protobuf::NewCallback(this, &BBSampleRpc::Done));
}

void
BBSampleRpc::Done()
{
  std::cerr << __func__ << std::endl;
}
