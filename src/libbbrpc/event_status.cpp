#include "event_status.h"

BeatBoard::EventStatus::EventStatus( void* instance, int status, void* client_event_status )
{
  this->instance = instance;
  this->status = status;
  this->client_event_status = client_event_status;
}

BeatBoard::EventStatus::~EventStatus()
{
}

void*
BeatBoard::EventStatus::getInstance()
{
  return instance;
}

int
BeatBoard::EventStatus::getStatus()
{
  return status;
}

void*
BeatBoard::EventStatus::getClientEventStatus()
{
  return client_event_status;
}
