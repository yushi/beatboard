#include "../sample.pb.h"
#include "../bb_rpc_controller.h"
#include "../bb_rpc_service.h"

class ExampleService : public sample::RpcService {
 public:
  void RpcFunc(google::protobuf::RpcController* controller,
               const sample::Request* request,
               sample::Response* response,
               google::protobuf::Closure* done);
};
