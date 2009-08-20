#include "rpc_server.h"
#include "searchapi_service.h"
#include "searchapi_service_rpc.h"

int main(int argc, char** argv)
{
  int port = 1235;
  std::string host = "127.0.0.1";
  BeatBoard::RpcServer* server = new BeatBoard::RpcServer(host);

  //google::protobuf::Service* service = new ExampleService;
  std::string db = "test";
  std::string table = "test2";
  std::string drizzle_host = "127.0.0.1";
  in_port_t drizzle_port = 8888;

  std::string memcached_host = "127.0.0.1";
  in_port_t memcached_port = 11211;

  searchapi::RpcService* service = new BeatBoard::SearchApiService( db, table, drizzle_host, drizzle_port, memcached_host, memcached_port );
  BeatBoard::BBRpcService* searchapiservicerpc = new BeatBoard::SearchApiServiceRpc( service );

  server->ExportOnPort(port, searchapiservicerpc);
  server->Run();

  delete service;
  return 0;
}
