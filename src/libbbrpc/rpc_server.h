#ifndef RPCSERVER_H
#define RPCSERVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <event.h>
#include <fcntl.h>
#include <vector>
#include <signal.h>
#include <google/protobuf/stubs/common.h>

#include "rpccommon.h"
#include "bb_rpc_service.h"
#include "event_status.h"
#include "client_event_status.h"

namespace BeatBoard {

  void EventHandller( int sockfd, short event, void *v);
//  static unsigned int BUFSIZE = 1024;

  class RpcServer {
  private:
    enum {
      libevent_accept,
      libevent_read
    };
    int server_sockfd;
    std::string host;
    BBRpcService* service;
    struct event_base *main_base;
    struct event server_event;
    std::vector<ClientEventStatus *> clients;

  public:
    RpcServer( const std::string& host );
    virtual ~RpcServer();
    void ExportOnPort( const int port, BBRpcService*& service );
    void Run();
    void Done();
    void Dispatch( int sockfd, short event, int status, void* clientklass );
    void Accept( int sockfd, short event );
    void Read( int sockfd, short event, ClientEventStatus* klass );
	//	void Finish();

  private:
    bool createSocket( const int port );
    void clientAdd( const int sockfd );
    void clientDelete( ClientEventStatus* klass );
    bool recvData( int sockfd );
    bool sendData( int sockfd );
	static int cleanUp();
	static void sigcb( int sig );
  };
}

#endif
