#include "rpc_server.h"
#include "logapi_service.h"
#include "logapi_service_rpc.h"
#include "api_common.h"

int main(int argc, char** argv)
{
  int port = 1234;
  std::string host = "127.0.0.1";
  BeatBoard::RpcServer* server = new BeatBoard::RpcServer(host);

  //google::protobuf::Service* service = new ExampleService;
  std::string db = "test";
  std::string table = "test2";
  std::string drizzle_host = "127.0.0.1";
  //in_port_t drizzle_port = 8888;
  in_port_t drizzle_port = 3306;
  std::string user = "user";
  std::string password = "";


  logapi::RpcService* service = new BeatBoard::LogApiService( db, table, drizzle_host, drizzle_port, BeatBoard::DB_MYSQL, user, password );
  BeatBoard::BBRpcService* logapiservicerpc = new BeatBoard::LogApiServiceRpc( service );

  server->ExportOnPort(port, logapiservicerpc);
  server->Run();

  delete service;
  return 0;
}
