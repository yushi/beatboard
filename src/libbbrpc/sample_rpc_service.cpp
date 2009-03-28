#include <iostream>
#include <fstream>
#include <string>

#include "sample.pb.h"
#include "bb_rpc_controller.h"
#include "rpc_server.h"
#include "bb_rpc_service.h"

class BBSampleRpc : public BeatBoard::BBRpcService {
private:
    sample::Response response;

public:
  BBSampleRpc( sample::RpcService* service ) : BBRpcService( service )
    {
      std::cout << __func__ << " constractor" << std::endl;
    }

  bool readRequest( const std::string& data )
    {
      std::cout << __func__ << " called" << std::endl;

      sample::Request request;
      if ( !request.ParseFromString( data ) ) {
        std::cout << "Failed to parse request." << std::endl;
        return false;
      }
      std::cout << "client query: " << request.query() << std::endl;
      callServiceFunc( request );

      return true;
    }

  bool writeResult( std::string& data )
    {
      std::cout << __func__ << " called" << std::endl;

      if ( !response.SerializeToString( &data ) ) {
        std::cout << "Failed to parse response." << std::endl;
        return false;
      }

      return true;
    }

  void callServiceFunc( sample::Request& request )
    {
      ((sample::RpcService*)service)->RpcFunc(new BeatBoard::BBRpcController(),
                                              &request, &response, 
                                              google::protobuf::NewCallback(this, &BBSampleRpc::Done));
    }

  void Done()
    {
      std::cerr << __func__ << std::endl;
    }

};

class ExampleService : public sample::RpcService {
 public:
  void RpcFunc(google::protobuf::RpcController* controller,
              const sample::Request* request,
              sample::Response* response,
              google::protobuf::Closure* done) {
    std::cout << __func__ << std::endl;
    if (request->query() == "query") 
    {
      response->set_result("query");
    }
    else 
    {
      response->set_result("not query");
    }
    done->Run();
  }

};

int main() {

  BeatBoard::RpcServer* server = new BeatBoard::RpcServer("127.0.0.1");

  //google::protobuf::Service* service = new ExampleService;
  sample::RpcService* service = new ExampleService;
  BeatBoard::BBRpcService* bbservice = new BBSampleRpc( service );

  server->ExportOnPort(1234, bbservice);
  server->Run();

  delete service;
  return 0;
}
