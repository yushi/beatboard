#include <sys/queue.h>
#include "bb_rpc_client_evhttp.h"

BBRpcClientEvhttp::BBRpcClientEvhttp( const std::string& host, const int port )
{
  this->host = host;
  this->port = port;
}

BBRpcClientEvhttp::~BBRpcClientEvhttp()
{
}

bool
BBRpcClientEvhttp::find_header( const struct evkeyvalq* headers,
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

bool
BBRpcClientEvhttp::start( const std::string& uri,
                          void (*cb)(struct evhttp_request *, void *),
                          void *arg)
{
  http_ev_base = event_init();
  httpd = evhttp_new( http_ev_base );

  httpd = evhttp_start( host.c_str(), port );
  if ( httpd )
  {
    evhttp_set_cb(httpd, "/", BBRpcClientEvhttp::rootHandler, NULL);
    evhttp_set_cb(httpd, uri.c_str(), cb, arg);

    event_dispatch();
    evhttp_free(httpd);

    return true;
  }
  std::cerr <<  "bind failed" << std::endl;
  return false;
}

void
BBRpcClientEvhttp::rootHandler(struct evhttp_request *req, void *arg)
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
