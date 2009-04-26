#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "authapi.pb.h"

//#include "sample_rpc_channel.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "authapi_service_common.h"

google::protobuf::RpcChannel* channel;
google::protobuf::RpcController* controller;
authapi::RpcService* service;
authapi::Request request;
authapi::Response response;

void Done2( authapi::Response* response_ ) {
  std::cout << "Done2" << std::endl;
  std::cout << "result: " << response_->result() << std::endl;
  std::cout << "error: " << response_->error() << std::endl;
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
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1236);
  controller = new BeatBoard::BBRpcController();

  service = new authapi::RpcService::Stub(channel);

  request.set_type(BeatBoard::AUTHAPI_ADD_USER);
  request.set_username("hoge");
  request.set_password("hoge");
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);
}


int main() {
  DoSearch();
  return 0;
}
