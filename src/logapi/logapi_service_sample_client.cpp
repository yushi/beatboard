#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "logapi.pb.h"

//#include "sample_rpc_channel.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "logapi_service_common.h"

google::protobuf::RpcChannel* channel;
google::protobuf::RpcController* controller;
logapi::RpcService* service;
logapi::Request request;
logapi::Response response;

void Done2( logapi::Response* response_ ) {
  std::cout << "Done2" << std::endl;
  std::cout << "result: " << response_->result() << std::endl;
  delete service;
  delete channel;
  delete controller;
}

void Done()
{
  std::cout << "Done1" << std::endl;
  Done2( &response );
}

void DoSearch() {
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1234);
  controller = new BeatBoard::BBRpcController();

  service = new logapi::RpcService::Stub(channel);

  request.set_channel("channel");
  request.set_time("20202002");
  request.set_identifier("identifier");
  request.set_message("hello!!");
  std::cout << "channel: " << request.channel() << std::endl;
  std::cout << "time: " << request.time() << std::endl;
  std::cout << "identifier: " << request.identifier() << std::endl;
  std::cout << "message: " << request.message() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);
}


int main() {
  DoSearch();
  return 0;
}
