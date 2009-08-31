#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

namespace BeatBoard {
  class Message
  {
  private:
    unsigned int expirationTIme;

  public:
    Message(const unsigned int expirationTIme);
    virtual ~Message();

    unsigned int getExpirationTime();
  };
}

#endif
