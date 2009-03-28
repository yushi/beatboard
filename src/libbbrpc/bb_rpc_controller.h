#ifndef BBRPCCONTROLLER_H
#define BBRPCCONTROLLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <google/protobuf/service.h>


namespace BeatBoard {
  class BBRpcController : public google::protobuf::RpcController {
  public:
    BBRpcController();
    virtual ~BBRpcController();

    void Reset();
    bool Failed() const;
    std::string ErrorText() const;
    void StartCancel();
    void SetFailed( const std::string& reason );
    bool IsCanceled() const;
    void NotifyOnCancel( google::protobuf::Closure* callback );
  };
}

#endif
