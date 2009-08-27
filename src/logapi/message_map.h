#ifndef MESSAGE_MAP_H
#define MESSAGE_MAP_H

#include <iostream>
#include <string>
#include <map>
#include <sys/time.h>

#include "message.h"

namespace BeatBoard {
  class MessageMap
  {
  private:
    std::map<std::string, Message*> data;
    unsigned int check_expiration_time;
    unsigned int check_expiration_interval;
    bool auto_expiration;
  
    Message* newMessage( const unsigned int expirationTIme );

  public:
    MessageMap( const unsigned int check_expiration_interval ) ;
    virtual ~MessageMap();

    bool setMessage( const std::string key, const unsigned int expirationTIme );
    bool checkMessageDuplication( const std::string key );
    void checkExpirationMessages();
    int size();
  };
}

#endif
