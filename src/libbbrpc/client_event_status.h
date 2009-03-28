#ifndef ClientEventStatus_H
#define ClientEventStatus_H

#include <unistd.h>
#include <string.h>
#include <event.h>

namespace BeatBoard {
  class ClientEventStatus {
  private:
    int sockfd;
    struct event ev;

  public:
    ClientEventStatus( int sockfd );
    ~ClientEventStatus();

    void eventSet( short flag, void (*callback)(int, short, void*), void *arg);
    void eventAdd( struct timeval *p );
    void terminate();
  };
}
#endif
