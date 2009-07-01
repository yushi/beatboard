#include "logapi_service_client_bbevqueue.h"

int main() {
  std::string rpcserver_host = "127.0.0.1";
  int rpcserver_port = 1234;

  BeatBoard::LogApiServiceClientBbevqueue* server = 
    new BeatBoard::LogApiServiceClientBbevqueue(
		rpcserver_host,
	    rpcserver_port
      );
  
  server->start();
  return 0;
}
