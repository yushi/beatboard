#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>

#include "searchapi.pb.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "searchapi_service_common.h"

#include "sample_rpc_client_evhttp.h"

google::protobuf::RpcChannel* channel;
google::protobuf::RpcController* controller;
searchapi::RpcService* service;
searchapi::Request request;
searchapi::Response response;

const std::string host = "127.0.0.1";
const int port = 1235;

void Done2( searchapi::Response* response_ ) {
  std::cout << "Done2" << std::endl;
  std::cout << "result code: " << response_->result_code() << std::endl;
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
  channel = new BeatBoard::BBRpcChannel(host, port);
  controller = new BeatBoard::BBRpcController();

  service = new searchapi::RpcService::Stub(channel);

  request.set_query(query);
  std::cout << "query: " << request.query() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);
}

const char* DoSearch(char *query) {
  Search(std::string(query));

  std::string result = "";
  if (response.result_code() == BeatBoard::SEARCHAPI_RESULT_OK)
  {
    result = response.result();
  }
  else if (response.result_code() == BeatBoard::SEARCHAPI_RESULT_ERROR)
  {
    result = "Search Faild";
  }
  else
  {
    result = "Unknow ret code";
  }
  return result.c_str();
}

void searchHandler( struct evhttp_request *req, void *arg )
{
  struct evbuffer *buf;
  const char *result;

  buf = evbuffer_new();
  if (buf) {
    result = DoSearch("e");
    evbuffer_add_printf(buf, "Hello Search Handler");
    evbuffer_add_printf(buf, result);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }
  else
  {
    std::cerr << "failed to create response buffer" << std::endl;
  }
}

int main() {
  std::string host = "127.0.0.1";
  int port = 8080;
  std::string uri = "/search";
  SampleRpcClientEvhttp* server =  new SampleRpcClientEvhttp(host, port);
  
  server->start(uri, &searchHandler);
  return 0;
}
