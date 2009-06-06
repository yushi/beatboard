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
  request.set_username("hoge7");
  request.set_password("hoge");
  request.set_userinfo("12345");
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;
  std::cout << "userinfo: " << request.userinfo() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);

  request.set_type(BeatBoard::AUTHAPI_VERIFY_USER);
  request.set_username("hoge7");
  request.set_password("hoge");
  request.set_userinfo("12345");
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;
  std::cout << "userinfo: " << request.userinfo() << std::endl;

  google::protobuf::Closure* callback2 = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback2);

  request.set_type(BeatBoard::AUTHAPI_UPDATE_USERINFO);
  request.set_username("hoge7");
  request.set_password("hoge");
  request.set_userinfo("12345");
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;
  std::cout << "userinfo: " << request.userinfo() << std::endl;

  google::protobuf::Closure* callback3 = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback3);

  delete service;
  delete channel;
  delete controller;
}


int main() {
  DoSearch();
  return 0;
}
