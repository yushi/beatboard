#ifndef BBRPCCHANNEL_H
#define BBRPCCHANNEL_H

#include <string>
#include <iostream>
#include <google/protobuf/service.h>
#include <google/protobuf/message.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "rpccommon.h"

namespace BeatBoard {

  class BBRpcChannel : public google::protobuf::RpcChannel {
  private:
    int sockfd;
    int result;
    struct sockaddr_in address;
    std::string host;
    int port;

  public:
    BBRpcChannel( const std::string& host, const int port );
    virtual ~BBRpcChannel();
    void CallMethod( const google::protobuf::MethodDescriptor* method,
                     google::protobuf::RpcController* controller,
                     const google::protobuf::Message* request,
                     google::protobuf::Message* response,
                     google::protobuf::Closure* done );
  };

}

#endif
