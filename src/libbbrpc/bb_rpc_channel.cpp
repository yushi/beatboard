#include "bb_rpc_channel.h"

BeatBoard::BBRpcChannel::BBRpcChannel( const std::string& host, const int port )
{
  std::cout << "host: " << host << std::endl;
  std::cout << "port: " << port << std::endl;

  this->host = host;
  this->port = port;
}

BeatBoard::BBRpcChannel::~BBRpcChannel()
{
}

void
BeatBoard::BBRpcChannel::CallMethod( const google::protobuf::MethodDescriptor* method,
                          google::protobuf::RpcController* controller,
                          const google::protobuf::Message* request,
                          google::protobuf::Message* response,
                          google::protobuf::Closure* done )
{  
  std::cout << "CallMethod" << std::endl;

  struct addrinfo hints;
  struct addrinfo *ai;
  int error;

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

  hints.ai_family = AF_UNSPEC;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_socktype = SOCK_STREAM;

  error = getaddrinfo( host.c_str(), NULL, &hints, &ai );
  if (error != 0)
  {
    perror("getaddrinfo");
    return;
  }

  if ((sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) == -1 )
  {
    perror("socket");
    freeaddrinfo(ai);
    return;
  }

  memset( &address, 0, sizeof(address) );
  address.sin_family = ai->ai_family;
  memcpy(ai->ai_addr, &address.sin_addr, ai->ai_addrlen);
  address.sin_port = htons( port );
  freeaddrinfo(ai);

  ssize_t len = sizeof(address);
  result = connect(sockfd, (struct sockaddr *)&address, len);
  if (result == -1)
  {
    std::cout << "connect failed" << std::endl;
    return;
  }

  std::string data;

  if ( !request->SerializeToString( &data ) ) {
    std::cout << "Failed to parse request." << std::endl;
    close(sockfd);
    return;
  }
  write(sockfd, data.c_str(), data.size());

  std::string recv_data;
  char buf[BUFSIZE];  
  do {
    memset(buf, 0, sizeof(buf));
    len = read(sockfd, &buf, sizeof(buf));
    recv_data += std::string(buf);
  } while (len < 0 && errno == EINTR );

  if ( !response->ParseFromString( recv_data ) ) {
    std::cout << "Failed to parse response." << std::endl;    
    close(sockfd);
    done->Run();
    return;
  }
  close(sockfd);

  done->Run();
}
