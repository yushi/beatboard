#include "rpc_server.h"
#include "authapi_service.h"
#include "authapi_service_rpc.h"
#include "api_common.h"

int main(int argc, char** argv)
{
  int port = 1236;
  std::string host = "127.0.0.1";
  BeatBoard::RpcServer* server = new BeatBoard::RpcServer(host);

  //google::protobuf::Service* service = new ExampleService;
  std::string db = "test";
  std::string table = "test3";
  std::string drizzle_host = "127.0.0.1";
  //in_port_t drizzle_port = 8888;
  in_port_t drizzle_port = 3306;
  std::string user = "user";
  std::string password = "";

  authapi::RpcService* service = new BeatBoard::AuthApiService( db, table, drizzle_host, drizzle_port, BeatBoard::DB_MYSQL, user, password );
  BeatBoard::BBRpcService* authapiservicerpc = new BeatBoard::AuthApiServiceRpc( service );

  server->ExportOnPort(port, authapiservicerpc);
  server->Run();

  delete service;
  return 0;
}
