#include <sys/queue.h>
#include "searchapi_service_client_evhttp.h"

SearchapiServiceClientEvhttp::SearchapiServiceClientEvhttp( 
  const std::string& evhttp_host,
  const int evhttp_port,
  const std::string& rpcserver_host,
  const int rpcserver_port
  )
{
  this->evhttp_host = evhttp_host;
  this->evhttp_port = evhttp_port;
  this->rpcserver_host = rpcserver_host;
  this->rpcserver_port = rpcserver_port;
  uri = "/search";
}

SearchapiServiceClientEvhttp::~SearchapiServiceClientEvhttp()
{
}

void
SearchapiServiceClientEvhttp::callback()
{
}

void
SearchapiServiceClientEvhttp::sendQuery( std::string query )
{
  channel = new BeatBoard::BBRpcChannel(rpcserver_host, rpcserver_port);
  controller = new BeatBoard::BBRpcController();

  service = new searchapi::RpcService::Stub(channel);

  request.set_query(query);
  std::cout << "query: " << request.query() << std::endl;

  google::protobuf::Closure* cb = google::protobuf::NewCallback(&SearchapiServiceClientEvhttp::callback);
  service->RpcFunc(controller, &request, &response, cb);

  std::cout << "result code: " << response.result_code() << std::endl;
  std::cout << "result: " << response.result() << std::endl;
  delete service;
  delete channel;
  delete controller;
}

std::string
SearchapiServiceClientEvhttp::search(char *query)
{
  sendQuery(std::string(query));

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
  return result;
}

void
SearchapiServiceClientEvhttp::searchHandler( struct evhttp_request *req, 
                                             void *arg )
{
  SearchapiServiceClientEvhttp *instance = static_cast<SearchapiServiceClientEvhttp*>( arg );
  struct evbuffer *buf;
  std::string result;

  buf = evbuffer_new();
  if (buf) {
    struct evkeyvalq headers;
    TAILQ_INIT(&headers);
    std::cerr << std::string(req->uri) << std::endl;
    evhttp_parse_query(req->uri, &headers);

    std::string key = "q";
    std::string val = "";

    if (BBRpcClientEvhttp::find_header(&headers, key, val))
    {
      char *escaped_val = evhttp_htmlescape(val.c_str());
      std::cerr << escaped_val << std::endl;
      result = instance->search(escaped_val);
      evhttp_add_header(req->output_headers, "Content-type","application/x-javascript; charset=utf-8");
      std::stringstream result_size;
      result_size << result.size();
      evhttp_add_header(req->output_headers, "Content-Length", result_size.str().c_str());
      evbuffer_add_printf(buf, result.c_str());
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

void
SearchapiServiceClientEvhttp::start()
{
  BBRpcClientEvhttp* server =  new BBRpcClientEvhttp(evhttp_host, evhttp_port);
  server->start(uri, &SearchapiServiceClientEvhttp::searchHandler, this);
}

