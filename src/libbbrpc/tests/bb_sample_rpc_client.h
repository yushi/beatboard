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
  void DoSearch();
  void DoSearch2(char *query);
  const char* DoSearch3(char *query);

};
