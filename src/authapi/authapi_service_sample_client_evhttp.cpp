#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <sys/queue.h>

#include "authapi.pb.h"

//#include "sample_rpc_channel.h"

#include "bb_rpc_channel.h"
#include "bb_rpc_controller.h"
#include "authapi_service_common.h"

#include "bb_rpc_client_evhttp.h"

google::protobuf::RpcChannel* channel;
google::protobuf::RpcController* controller;
authapi::RpcService* service;
authapi::Request request;
authapi::Response response;

void Done2( authapi::Response* response_ ) {
  std::cout << "Done2" << std::endl;
  std::cout << "result: " << response_->result() << std::endl;
  std::cout << "error: " << response_->error() << std::endl;
}

void Done()
{
  std::cout << "Done1" << std::endl;
  Done2( &response );
}

void updateUserInfo()
{
  request.set_type(BeatBoard::AUTHAPI_UPDATE_USERINFO);
  request.set_username("hoge7");
  request.set_password("hoge");
  request.set_userinfo("12345");
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;
  std::cout << "userinfo: " << request.userinfo() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);
}

unsigned int
verifyUser( std::map<std::string, std::string> params )
{
  request.set_type(BeatBoard::AUTHAPI_VERIFY_USER);
  request.set_username(params["name"]);
  request.set_password(params["pass"]);
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);

  return response.result();
}

unsigned int
addUser( std::map<std::string, std::string> params )
{
  request.set_type(BeatBoard::AUTHAPI_ADD_USER);
  request.set_username(params["name"]);
  request.set_password(params["pass"]);
  request.set_userinfo(params["mail"]);
  std::cout << "username: " << request.username() << std::endl;
  std::cout << "password: " << request.password() << std::endl;

  google::protobuf::Closure* callback = google::protobuf::NewCallback(&Done);
  service->RpcFunc(controller, &request, &response, callback);

  return response.result();
}

std::map<std::string, std::string>
parseHeaders( struct evkeyvalq& headers )
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

void authHandler( struct evhttp_request *req, void *arg )
{
  channel = new BeatBoard::BBRpcChannel("127.0.0.1", 1236);
  controller = new BeatBoard::BBRpcController();
  service = new authapi::RpcService::Stub(channel);

  //verifyUser();
  //updateUserInfo();

  struct evbuffer *buf = evbuffer_new();
  if (buf) {
    struct evkeyvalq headers;
    TAILQ_INIT(&headers);
    std::cerr << std::string(req->uri) << std::endl;
    evhttp_parse_query(req->uri, &headers);

    std::map<std::string, std::string> params = parseHeaders(headers);

    if (params["name"] != "" && params["pass"] != "")
    {
      std::cerr << "query correct" << std::endl;
      std::stringstream result;

      if (params["mode"] == "ADD_USER" && params["mail"] != "")
      {
        result << addUser(params);
      }
      else if (params["mode"] == "VERIFY_USER")
      {
        result << verifyUser(params);
      }

      std::string result_json = "{result:" + result.str() + "}";
      evbuffer_add_printf(buf, result_json.c_str());
    }
    else
    {
      std::cerr << "not find" << std::endl;
    }
    //evbuffer_add_printf(buf, "query received");
    evhttp_clear_headers(&headers);
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }
  else
  {
    std::cerr << "failed to create response buffer" << std::endl;
  }

  delete service;
  delete channel;
  delete controller;
}


int main() {
  std::string host = "127.0.0.1";
  int port = 8082;
  std::string uri = "/auth";
  BBRpcClientEvhttp* server =  new BBRpcClientEvhttp(host, port);
  
  server->start(uri, &authHandler, NULL);

  return 0;
}
