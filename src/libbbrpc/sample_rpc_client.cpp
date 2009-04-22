#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "sample.pb.h"

//#include "sample_rpc_channel.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"

google::protobuf::RpcChannel* channel;
google::protobuf::RpcController* controller;
sample::RpcService* service;
sample::Request request;
sample::Response response;

void Done2( sample::Response* response_ ) {
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

void Search(std::string query) {
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1234);
  controller = new BeatBoard::BBRpcController();

  service = new sample::RpcService::Stub(channel);

  request.set_query(query);
  std::cout << "query: " << request.query() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);
}

void DoSearch() {
  Search("query from client");
}

void DoSearch2(char *query) {
  Search(std::string(query));
}

const char* DoSearch3(char *query) {
  Search(std::string(query));
  std::string result = response.result();
  return result.c_str();
}

int main() {
  DoSearch();
  return 0;
}
