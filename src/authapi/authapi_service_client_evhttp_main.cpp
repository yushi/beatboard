#include "authapi_service_client_evhttp.h"

int main() {
  std::string evhttp_host = "127.0.0.1";
  int evhttp_port = 8082;
  std::string rpcserver_host = "127.0.0.1";
  int rpcserver_port = 1236;
  std::string memcached_host = "127.0.0.1";
  int memcached_port = 11211;


  AuthapiServiceClientEvhttp* server =  new AuthapiServiceClientEvhttp(
    evhttp_host, 
    evhttp_port,
    rpcserver_host,
    rpcserver_port,
    memcached_host,
    memcached_port
    );
  
  server->start();
  return 0;
}

