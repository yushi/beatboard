#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "searchapi.pb.h"

//#include "sample_rpc_channel.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "searchapi_service_common.h"

google::protobuf::RpcChannel* channel;
google::protobuf::RpcController* controller;
searchapi::RpcService* service;
searchapi::Request request;
searchapi::Response response;

void Done2( searchapi::Response* response_ ) {
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

void Search( std::string query ) {
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1235);
  controller = new BeatBoard::BBRpcController();

  service = new searchapi::RpcService::Stub(channel);

  request.set_query(query);
  std::cout << "query: " << request.query() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);
}

const char* DoSearch(char *query) {
  Search(std::string(query));
  std::string result = response.result();
  return result.c_str();
}

void DoSearchDummy() {
  Search("e");
}


int main() {
  DoSearchDummy();
  return 0;
}
