#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../sample.pb.h"
#include "../bb_rpc_channel.h"
#include "../bb_rpc_controller.h"

class BBSampleRpcClient {
private:
  google::protobuf::RpcChannel* channel;
  google::protobuf::RpcController* controller;
  sample::RpcService* service;
  sample::Request request;
  sample::Response response;

public:
  BBSampleRpcClient();
  virtual ~BBSampleRpcClient();
  static void Done();
  void Search(std::string query);
  std::string DoSearch();
  std::string DoSearch2(std::string& query);
  const char* DoSearch3(char *query);

};
