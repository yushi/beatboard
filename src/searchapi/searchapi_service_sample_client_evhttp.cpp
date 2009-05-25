#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/queue.h>

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

void debug( searchapi::Response* response_ ) {
  std::cout << "Done2" << std::endl;
  std::cout << "result code: " << response_->result_code() << std::endl;
  std::cout << "result: " << response_->result() << std::endl;
  delete service;
  delete channel;
  delete controller;
}

void callback()
{
  debug( &response );
}

void Search( std::string query ) {
  channel = new BeatBoard::BBRpcChannel(host, port);
  controller = new BeatBoard::BBRpcController();

  service = new searchapi::RpcService::Stub(channel);

  request.set_query(query);
  std::cout << "query: " << request.query() << std::endl;

  google::protobuf::Closure* cb = google::protobuf::NewCallback(&callback);
  service->RpcFunc(controller, &request, &response, cb);
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
    // Search Failed
    result = "{}";
  }
  else
  {
    // Unknown ret code
    result = "{}";
  }
  return result.c_str();
}

void searchHandler( struct evhttp_request *req, void *arg )
{
  struct evbuffer *buf;
  const char *result;

  buf = evbuffer_new();
  if (buf) {
    struct evkeyvalq headers;
    TAILQ_INIT(&headers);
    std::cerr << std::string(req->uri) << std::endl;
    evhttp_parse_query(req->uri, &headers);

    std::string key = "q";
    std::string val = "";

    if (SampleRpcClientEvhttp::find_header(&headers, key, val))
    {
      char *escaped_val = evhttp_htmlescape(val.c_str());
      std::cerr << escaped_val << std::endl;
      result = DoSearch(escaped_val);
      evbuffer_add_printf(buf, result);
      free(escaped_val);
    }
    else
    {
      std::cerr << "not find" << std::endl;
    }
    evhttp_clear_headers(&headers);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }
  else
  {
    std::cerr << "failed to create response buffer" << std::endl;
  }
}

int main() {
  std::string host = "127.0.0.1";
  int port = 8081;
  std::string uri = "/search";
  SampleRpcClientEvhttp* server =  new SampleRpcClientEvhttp(host, port);
  
  server->start(uri, &searchHandler);
  return 0;
}
