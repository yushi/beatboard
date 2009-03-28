#include "client_event_status.h"

BeatBoard::ClientEventStatus::ClientEventStatus( int sockfd )
{
  this->sockfd = sockfd;
  memset(&ev, 0 , sizeof(ev));
}

BeatBoard::ClientEventStatus::~ClientEventStatus()
{
  terminate();
}

void
BeatBoard::ClientEventStatus::eventSet( short flag, void (*callback)(int, short, void*), void *arg )
{
  event_set(&ev, sockfd, flag, callback, arg);
}

void
BeatBoard::ClientEventStatus::eventAdd( struct timeval *p )
{
  event_add(&ev, p);
}

void
BeatBoard::ClientEventStatus::terminate()
{
  if (event_initialized(&ev))
  {
    event_del(&ev);
  }
  close(sockfd);
}
