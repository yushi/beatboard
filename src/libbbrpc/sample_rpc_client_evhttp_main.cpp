#include <string>
#include <iostream>

#include "sample_rpc_client_evhttp.h"

void testHandler( struct evhttp_request *req, void *arg )
{
  struct evbuffer *buf;
  buf = evbuffer_new();
  if (buf) {
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
  SampleRpcClientEvhttp* server =  new SampleRpcClientEvhttp(host, port);
  
  server->start(uri, &testHandler);
  return 0;
}
