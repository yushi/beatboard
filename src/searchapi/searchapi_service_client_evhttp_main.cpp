#include "searchapi_service_client_evhttp.h"

int main() {
  std::string evhttp_host = "127.0.0.1";
  int evhttp_port = 8081;
  std::string rpcserver_host = "127.0.0.1";
  int rpcserver_port = 1235;

  SearchapiServiceClientEvhttp* server =  new SearchapiServiceClientEvhttp(
    evhttp_host, 
    evhttp_port,
    rpcserver_host,
    rpcserver_port);
  
  server->start();
  return 0;
}
