#include "rpc_server.h"
#include "logapi_service.h"
#include "logapi_service_rpc.h"

int main(int argc, char** argv)
{
  int port = 1234;
  std::string host = "127.0.0.1";
  BeatBoard::RpcServer* server = new BeatBoard::RpcServer(host);

  //google::protobuf::Service* service = new ExampleService;
  logapi::RpcService* service = new BeatBoard::LogApiService;
  BeatBoard::BBRpcService* logapiservicerpc = new BeatBoard::LogApiServiceRpc( service );

  server->ExportOnPort(port, logapiservicerpc);
  server->Run();

  delete service;
  return 0;
}
