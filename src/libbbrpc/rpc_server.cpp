#include "rpc_server.h"

void
BeatBoard::EventHandller( int sockfd, short event, void *v )
{
    std::cerr << "called libevent handler" << std::endl;
    //((RpcServer*) v)->Accept(sockfd, event);
    void* klass = ((EventStatus*) v)->getInstance();
    int status = ((EventStatus*) v)->getStatus();
    void* clientklass = ((EventStatus*) v)->getClientEventStatus();
    ((RpcServer*) klass)->Dispatch(sockfd, event, status, clientklass);
}

BeatBoard::RpcServer::RpcServer( const std::string& host )
{
  this->host = host;
}

BeatBoard::RpcServer::~RpcServer()
{
}

void
BeatBoard::RpcServer::Done()
{
  std::cerr << "Done" << std::endl;
}

void
BeatBoard::RpcServer::Dispatch( int sockfd, short event, int status, void* clientklass )
{
  std::cerr << "case: " << status << std::endl;
  switch(status)
  {
  case libevent_accept:
    Accept(sockfd, event);
    break;
  case libevent_read:
    Read(sockfd, event, static_cast<ClientEventStatus *>(clientklass));
    break;
  default:
    break;
  }
}

void
BeatBoard::RpcServer::Accept( int sockfd, short event )
{
    std::cerr << "called server accept" << std::endl;
    std::cerr << sockfd << ":" << event << std::endl;
    if ( event & EV_READ )
    {
      std::cerr << "accept here" << std::endl;
      int client_sockfd;
      struct sockaddr_in client_address;
      socklen_t client_len = sizeof( client_address );

      if ( (client_sockfd = accept(sockfd, (struct sockaddr *)&client_address, &client_len)) == -1 )
      {
        perror("accept");
        return;
      }

      int flags;
      if ((flags = fcntl(client_sockfd, F_GETFL, 0)) < 0 ||
          fcntl(client_sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
      {
        perror("fcntl O_NONBLOCK");
        close(server_sockfd);
        return;
      }

//       struct event client_event;
//       EventStatus *es = new EventStatus(this, libevent_read);
//       event_set(&client_event, client_sockfd, EV_READ | EV_PERSIST, EventHandller, es);
//       event_add(&client_event, NULL);
      clientAdd(client_sockfd);

    }
}

void
BeatBoard::RpcServer::clientAdd( const int sockfd )
{
  std::cerr << __func__ << std::endl;
  ClientEventStatus* ces = new ClientEventStatus(sockfd);
  std::cerr << "ces created" << std::endl;

  EventStatus *es = new EventStatus(this, libevent_read, ces);
  ces->eventSet(EV_READ | EV_PERSIST, EventHandller, es);
  ces->eventAdd(NULL);
  clients.push_back(ces);
}

void
BeatBoard::RpcServer::Read( int sockfd, short event, ClientEventStatus* ces )
{
  std::cerr << "called server Read" << std::endl;
  if ( event & EV_READ )
  {
    if (!recvData(sockfd))
    {
      std::cerr << "Failed recvData." << std::endl;
      clientDelete(ces);
      return;
    }
    
    if (!sendData(sockfd))
    {
      std::cerr << "Failed sendData." << std::endl;
      clientDelete(ces);
      return;
    }

    clientDelete(ces);
  }

}

bool 
BeatBoard::RpcServer::sendData( int sockfd )
{
  std::string data;
  if ( !service->writeResult(data)) 
  {
    std::cerr << "Failed to writeResult()." << std::endl;
    close(sockfd);
    return false;
  }

  std::cerr << "Data: " << data << std::endl;
  std::cerr << "Data len: " << data.length() << std::endl;
  std::stringstream ss;
  ss << data.length();
  std::string data_length_str = ss.str();
  char length_buf[BUFSIZE];
  ssize_t len;
  do {
    memset(length_buf, 0, sizeof(length_buf));
    strncpy(length_buf, data_length_str.c_str(), data_length_str.length());
    len = write(sockfd, &length_buf, sizeof(length_buf));
  } while ( len < 0 && errno == EINTR );

  do {
    len = write(sockfd, data.c_str(), data.size());
    std::cerr << "len: " << len << std::endl;
  } while ( len < 0 && errno == EINTR );

  std::cout << "send response" << std::endl;

  return true;
}

bool
BeatBoard::RpcServer::recvData( int sockfd )
{
  char buf[BUFSIZE];
  std::string data;
  ssize_t len;
  do
  {
    memset(buf, 0, sizeof(buf));
    len = read(sockfd, &buf, sizeof(buf));
    data += std::string(buf);
  } while (len < 0 && errno == EINTR );
  std::cerr << data << std::endl;

  if ( !service->readRequest(data) ) {
    std::cerr << "Failed readRequest." << std::endl;
    close(sockfd);
    return false;
  }
  
  return true;
}

void
BeatBoard::RpcServer::clientDelete( ClientEventStatus* ces )
{
  std::vector<ClientEventStatus *>::iterator it;

  for( it = clients.begin(); it != clients.end(); ++it )
  {
    std::cout << *it << std::endl;
    if ((*it) == ces)
    {
      ces->terminate();
      delete ces;
      clients.erase(it);
      break;
    }
  }
}

void
BeatBoard::RpcServer::ExportOnPort( const int port, BBRpcService*& service )
{
  std::cout << "port: " << port << std::endl;
  this->service = service;
  main_base = event_init();
  memset(&server_event, 0, sizeof(server_event));
  if (!createSocket(port))
  {
    perror("createSocket");
    abort();
  }
}

void
BeatBoard::RpcServer::Run()
{
  std::cerr << "libevent server start1."  << std::endl;
  event_dispatch();
//  std::cerr << "libevent server start2."  << std::endl;
//  event_base_loop(main_base, 0);
}

bool
BeatBoard::RpcServer::createSocket( const int port )
{
  int flags;
  int error;
  struct addrinfo hints;
  struct addrinfo *ai;
  struct addrinfo *next;
//  char port_buf[NI_MAXSERV];

  memset(&hints, 0, sizeof(hints));
  hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;

  hints.ai_family = AF_UNSPEC;
  hints.ai_protocol = IPPROTO_TCP;
  hints.ai_socktype = SOCK_STREAM;

  std::ostringstream ostr;
  ostr << port;

  //std::cerr << port_buf << std::endl;
  //strncpy(port_buf, ostr.str().c_str(), ostr.str().size());
  //std::cerr << ostr.str().size() << std::endl;
  //error = getaddrinfo( host.c_str(), port_buf, &hints, &ai );
  error = getaddrinfo( host.c_str(), ostr.str().c_str(), &hints, &ai );
  if (error != 0)
  {
    perror("getaddrinfo");
    return false;
  }

  for (next = ai; next; next = next->ai_next)
  {
    std::cout << "address: " << next->ai_addr << std::endl;
    if ((server_sockfd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol)) == -1 )
    {
      perror("socket");
      freeaddrinfo(ai);
      return false;
    }

    if ((flags = fcntl(server_sockfd, F_GETFL, 0)) < 0 ||
        fcntl(server_sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    {
      perror("fcntl O_NONBLOCK");
      freeaddrinfo(ai);
      close(server_sockfd);
      return false;
    }

    int on = 1;
    struct linger ling = {0, 0};
    setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, (void *)&on, sizeof(on));
    setsockopt(server_sockfd, SOL_SOCKET, SO_KEEPALIVE, (void *)&on, sizeof(on));
    setsockopt(server_sockfd, SOL_SOCKET, SO_LINGER, (void *)&ling, sizeof(ling));
    setsockopt(server_sockfd, IPPROTO_TCP, TCP_NODELAY, (void *)&on, sizeof(on));
    if (bind(server_sockfd, next->ai_addr, next->ai_addrlen) == -1)
    {
      if (errno != EADDRINUSE)
      {
        perror("bind");
        freeaddrinfo(ai);
        close(server_sockfd);
        return false;
      }
      close(server_sockfd);
      continue;
    }
    else
    {
      if (listen(server_sockfd, 1024) == -1)
      {
        perror("listen");
        freeaddrinfo(ai);
        close(server_sockfd);
        return false;
      }
    }
    
//    event_set(&server_event, server_sockfd, EV_READ | EV_PERSIST, EventHandller, this);
    EventStatus *es = new EventStatus(this, libevent_accept, NULL);
    event_set(&server_event, server_sockfd, EV_READ | EV_PERSIST, EventHandller, es);
//    event_base_set(main_base, &server_event);
    event_add(&server_event, NULL);
    std::cout <<  "event_method : " << event_get_method() << std::endl;
  }

  freeaddrinfo(ai);

  return true;
}
