#include <sys/queue.h>
#include "sample_rpc_client_evhttp.h"


SampleRpcClientEvhttp::SampleRpcClientEvhttp( const std::string& host, const int port )
{
  this->host = host;
  this->port = port;
}

SampleRpcClientEvhttp::~SampleRpcClientEvhttp()
{
}

bool
SampleRpcClientEvhttp::start( const std::string& uri,
                              void (*cb)(struct evhttp_request *, void *))
{
  http_ev_base = event_init();
  httpd = evhttp_new( http_ev_base );

  httpd = evhttp_start( host.c_str(), port );
  if ( httpd )
  {
    evhttp_set_cb(httpd, "/", SampleRpcClientEvhttp::rootHandler, NULL);
    evhttp_set_cb(httpd, uri.c_str(), cb, NULL);

    event_dispatch();
    evhttp_free(httpd);

    return true;
  }
  std::cerr <<  "bind failed" << std::endl;
  return false;
}

void
SampleRpcClientEvhttp::rootHandler(struct evhttp_request *req, void *arg)
{
  struct evbuffer *buf;
  buf = evbuffer_new();
  if (buf) {
    evbuffer_add_printf(buf, "Hello evhttp World!");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }
  else
  {
    //fprintf(stderr, "failed to create response buffer\n");
    std::cerr << "failed to create response buffer" << std::endl;
  }
}
