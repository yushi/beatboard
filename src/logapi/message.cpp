#include "message.h"

BeatBoard::Message::Message( const unsigned int expirationTIme )
{      
  this->expirationTIme = expirationTIme;
}

BeatBoard::Message::~Message()
{
}

unsigned int
BeatBoard::Message::getExpirationTime()
{
  return expirationTIme;
}

