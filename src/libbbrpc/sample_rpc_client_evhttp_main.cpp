#include <string>
#include <iostream>
#include <sys/queue.h>

#include "bb_rpc_client_evhttp.h"

void testHandler( struct evhttp_request *req, void *arg )
{
  struct evbuffer *buf;
  buf = evbuffer_new();
  if (buf) 
  {
    struct evkeyvalq headers;
    TAILQ_INIT(&headers);
    std::cerr << std::string(req->uri) << std::endl;
    evhttp_parse_query(req->uri, &headers);

    std::string key = "test";
    std::string val = "";
    if (BBRpcClientEvhttp::find_header(&headers, key, val))
    {
      char *escaped_val = evhttp_htmlescape(val.c_str());
      std::cerr << escaped_val << std::endl;
      free(escaped_val);
    }
    else
    {
      std::cerr << "not find" << std::endl;
    }
    evhttp_clear_headers(&headers);

    evbuffer_add_printf(buf, "Hello Test Handler");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }
  else
  {
    std::cerr << "failed to create response buffer" << std::endl;
  }
}

int main (int argc, char *argv[])
{
  std::string host = "127.0.0.1";
  int port = 8080;
  std::string uri = "/test";
  BBRpcClientEvhttp* server =  new BBRpcClientEvhttp(host, port);
  
  server->start(uri, &testHandler, NULL);
  return 0;
}
