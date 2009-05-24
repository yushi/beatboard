#include <string>
#include <iostream>
#include <sys/queue.h>

#include "sample_rpc_client_evhttp.h"

bool find_header( const struct evkeyvalq* headers,
                  const std::string& key,
                  std::string& val )
{
  bool ret = false;

  const char *query_val = evhttp_find_header(headers, key.c_str());

  if (query_val != NULL)
  {
    std::cerr << query_val << std::endl;
    val = std::string(query_val);
    ret = true;
  }
  return ret;
}

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
    if (find_header(&headers, key, val))
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
  SampleRpcClientEvhttp* server =  new SampleRpcClientEvhttp(host, port);
  
  server->start(uri, &testHandler);
  return 0;
}
