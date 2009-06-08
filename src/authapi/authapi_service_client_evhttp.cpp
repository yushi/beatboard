#include <sys/queue.h>
#include <evhttp.h>
#include "authapi_service_client_evhttp.h"

AuthapiServiceClientEvhttp::AuthapiServiceClientEvhttp( 
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
  uri = "/auth";
  srand(time(0));
}

AuthapiServiceClientEvhttp::~AuthapiServiceClientEvhttp()
{
}

void
AuthapiServiceClientEvhttp::callback()
{
}

void
AuthapiServiceClientEvhttp::setUpRpc()
{
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1236);
  controller = new BeatBoard::BBRpcController();
  service = new authapi::RpcService::Stub(channel);
}

void
AuthapiServiceClientEvhttp::deleteRpc()
{
  delete service;
  delete channel;
  delete controller;
}

unsigned int
AuthapiServiceClientEvhttp::updateUserInfo( 
  std::map<std::string, std::string> params 
  )
{
  setUpRpc();
  request.set_type(BeatBoard::AUTHAPI_UPDATE_USERINFO);
  request.set_username("hoge7");
  request.set_password("hoge");
  request.set_userinfo("12345");
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;
  std::cout << "userinfo: " << request.userinfo() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&AuthapiServiceClientEvhttp::callback);
  service->RpcFunc(controller, &request, &response, callback);
  deleteRpc();
}
 
unsigned int 
AuthapiServiceClientEvhttp::verifyUser( 
  std::map<std::string, std::string> params 
)
{
  setUpRpc();
  request.set_type(BeatBoard::AUTHAPI_VERIFY_USER);
  request.set_username(params["name"]);
  request.set_password(params["pass"]);
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&AuthapiServiceClientEvhttp::callback);
  service->RpcFunc(controller, &request, &response, callback);

  deleteRpc();
  return response.result();
}
 
unsigned int
AuthapiServiceClientEvhttp::addUser(
  std::map<std::string, std::string> params
)
{
  setUpRpc();
  request.set_type(BeatBoard::AUTHAPI_ADD_USER);
  request.set_username(params["name"]);
  request.set_password(params["pass"]);
  request.set_userinfo(params["mail"]);
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&AuthapiServiceClientEvhttp::callback);
  service->RpcFunc(controller, &request, &response, callback);

  deleteRpc();
  return response.result();
}

std::map<std::string, std::string>
AuthapiServiceClientEvhttp::parseHeaders( struct evkeyvalq& headers )
{
  std::map<std::string, std::string> params;
  std::string name_key = "n";
  std::string name = "";
  std::string pass_key = "p";
  std::string pass = "";
  std::string mail_key = "m";
  std::string mail = "";
  std::string name_mode = "mode";
  std::string mode = "";

  BBRpcClientEvhttp::find_header(&headers, name_key, name);
  BBRpcClientEvhttp::find_header(&headers, pass_key, pass);
  BBRpcClientEvhttp::find_header(&headers, mail_key, mail);
  BBRpcClientEvhttp::find_header(&headers, name_mode, mode);

  char *escaped_name = evhttp_htmlescape(name.c_str());
  char *escaped_pass = evhttp_htmlescape(pass.c_str());
  char *escaped_mail = evhttp_htmlescape(mail.c_str());
  char *escaped_mode = evhttp_htmlescape(mode.c_str());

  params.insert(std::pair<std::string, std::string>("name", 
                                                    std::string(escaped_name)));
  params.insert(std::pair<std::string, std::string>("pass", 
                                                    std::string(escaped_pass)));
  params.insert(std::pair<std::string, std::string>("mail", 
                                                    std::string(escaped_mail)));
  params.insert(std::pair<std::string, std::string>("mode", 
                                                    std::string(escaped_mode)));

  free(escaped_name);
  free(escaped_pass);
  free(escaped_mail);
  free(escaped_mode);

  return params;
}

void
AuthapiServiceClientEvhttp::authHandler( struct evhttp_request *req, void *arg )
{
  AuthapiServiceClientEvhttp *instance = static_cast<AuthapiServiceClientEvhttp*>( arg );

  struct evbuffer *buf = evbuffer_new();
  if (buf) {
    struct evkeyvalq headers;
    TAILQ_INIT(&headers);
    std::cerr << std::string(req->uri) << std::endl;
    evhttp_parse_query(req->uri, &headers);

    std::map<std::string, std::string> params = instance->parseHeaders(headers);

    if (params["name"] != "" && params["pass"] != "")
    {
      std::cerr << "query correct" << std::endl;
      std::string result_json;
      std::stringstream result;

      if (params["mode"] == "ADD_USER" && params["mail"] != "")
      {
        result << instance->addUser(params);
        result_json = "{result:" + result.str() + "}";
      }
      else if (params["mode"] == "VERIFY_USER")
      {
        std::string sid = "";
        sid = BBRpcClientEvhttp::uniq_id();
        result << instance->verifyUser(params);
        result_json = "{result:" + result.str() + ", sid:" + sid + "}";
        std::cerr << result_json << std::endl;
        // push sid to memcached
      }
      evbuffer_add_printf(buf, result_json.c_str());
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
  evbuffer_free(buf);
}
 
void
AuthapiServiceClientEvhttp::start()
{
  BBRpcClientEvhttp* server =  new BBRpcClientEvhttp(evhttp_host, evhttp_port);
  server->start(uri, &AuthapiServiceClientEvhttp::authHandler, this);
}

