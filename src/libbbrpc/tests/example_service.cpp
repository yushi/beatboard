#include "example_service.h"

ExampleService::ExampleService()
{
}

ExampleService::~ExampleService()
{
}

void
ExampleService::RpcFunc(google::protobuf::RpcController* controller,
                        const sample::Request* request,
                        sample::Response* response,
                        google::protobuf::Closure* done)
{
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
