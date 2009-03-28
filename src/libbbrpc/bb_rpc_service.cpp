#include "bb_rpc_service.h"

BeatBoard::BBRpcService::BBRpcService( void *service )
{
  this->service = service;
}

BeatBoard::BBRpcService::~BBRpcService()
{
}

bool
BeatBoard::BBRpcService::readRequest( const std::string& data )
{
  return true;
}

bool
BeatBoard::BBRpcService::writeResult( std::string& data )
{
  return true;
}


void
BeatBoard::BBRpcService::Done()
{
  std::cerr << __func__ << std::endl;
}
