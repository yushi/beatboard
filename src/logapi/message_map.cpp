#include "message_map.h"

BeatBoard::MessageMap::MessageMap( const unsigned int check_expiration_interval ) 
{
  this->check_expiration_interval = check_expiration_interval;
  check_expiration_time = (unsigned int)time(NULL) + check_expiration_interval;
  auto_expiration = true;
}

BeatBoard::MessageMap::~MessageMap()
{
  std::map<std::string, Message*>::iterator it = data.begin();
  for(; it != data.end(); it++)
  {
    std::cerr << it->first << " : " << it->second->getExpirationTime() << std::endl;
    delete(it->second);
    data.erase(it);
  }
}


BeatBoard::Message*
BeatBoard::MessageMap::newMessage( const unsigned int expirationTIme )
{
  Message *message = new Message(expirationTIme);
  return message;
}


bool
BeatBoard::MessageMap::setMessage( const std::string key, const unsigned int expirationTIme )
{
  bool result;

  if (!checkMessageDuplication(key))
  {
    result = true;
  }
  else
  {
    result = false;
  }
  data[key] = newMessage( (unsigned int)time(NULL) + expirationTIme );
  return result;
}

bool
BeatBoard::MessageMap::checkMessageDuplication( const std::string key )
{
  std::map<std::string, Message*>::iterator it = data.find(key);
  bool result;

  if (it != data.end())
  {
    std::cerr << it->second->getExpirationTime() << std::endl;
    delete(it->second);
    data.erase(it);
    result = true;
  }
  else
  {
    std::cerr << key << " not exists" << std::endl;
    result = false;
  }
  if (auto_expiration)
  {
    checkExpirationMessages();
  }

  return result;
}

void
BeatBoard::MessageMap::checkExpirationMessages()
{
  unsigned int current = (unsigned int)time(NULL);
      
  std::cerr << "time1: " << check_expiration_time << std::endl;
  std::cerr << "time2: " << current << std::endl;
  std::cerr << "check: " << (check_expiration_time < current && !data.empty()) << std::endl;

  if (check_expiration_time < current && !data.empty())
  {
    std::cerr << __func__ << std::endl;
    std::map<std::string, Message*>::iterator it = data.begin();
    for(; it != data.end(); it++)
    {
      std::cerr << it->first << " : " << it->second->getExpirationTime() << std::endl;
      if (it->second->getExpirationTime() <= current)
      {
        delete(it->second);
        data.erase(it);
      }
    }
    check_expiration_time = current + check_expiration_interval;
  }
}

int
BeatBoard::MessageMap::size()
{
  return data.size();
}
