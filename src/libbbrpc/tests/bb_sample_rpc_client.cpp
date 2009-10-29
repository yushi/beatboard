#include "bb_sample_rpc_client.h"

BBSampleRpcClient::BBSampleRpcClient()
{
}

BBSampleRpcClient::~BBSampleRpcClient()
{
}

void
BBSampleRpcClient::Done()
{
}

void
BBSampleRpcClient::Search(std::string query) 
{
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1234);
  controller = new BeatBoard::BBRpcController();

  service = new sample::RpcService::Stub(channel);

  request.set_query(query);
  std::cout << "query: " << request.query() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&BBSampleRpcClient::Done);
  service->RpcFunc(controller, &request, &response, callback);

  std::cout << "Done" << std::endl;
  std::cout << "result: " << response.result() << std::endl;
  delete service;
  delete channel;
  delete controller;
}

void
BBSampleRpcClient::DoSearch()
{
  Search("query from client");
}

void
BBSampleRpcClient::DoSearch2(char *query)
{
  Search(std::string(query));
}

const char*
BBSampleRpcClient::DoSearch3(char *query)
{
  Search(std::string(query));
  std::string result = response.result();
  return result.c_str();
}
