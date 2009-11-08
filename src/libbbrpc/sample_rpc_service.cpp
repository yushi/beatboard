#include <iostream>
#include <fstream>
#include <string>

#include "sample.pb.h"
#include "bb_rpc_controller.h"
#include "rpc_server.h"
#include "bb_rpc_service.h"

#include "./tests/bb_sample_rpc.h"
#include "./tests/example_service.h"

int main() {

  BeatBoard::RpcServer* server = new BeatBoard::RpcServer("127.0.0.1");

  //google::protobuf::Service* service = new ExampleService;
  sample::RpcService* service = new ExampleService();
  BeatBoard::BBRpcService* bbservice = new BBSampleRpc( service );

  server->ExportOnPort(1234, bbservice);
  server->Run();
  std::cerr << "server finished" << std::endl;
  delete service;
  service = NULL;
  delete server;
  server = NULL;
  
  return 0;
}
