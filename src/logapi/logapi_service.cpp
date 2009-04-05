#include "logapi_service.h"

void
BeatBoard::LogApiService::RpcFunc(google::protobuf::RpcController* controller,
                                  const logapi::Request* request,
                                  logapi::Response* response,
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
