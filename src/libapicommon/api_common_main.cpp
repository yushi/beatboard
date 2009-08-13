#include <iostream>

#include "api_common.h"
#include "api_memcached.h"

int main()
{
  std::string query = "h,o\"g'e h,o\"g'e";
  std::cout << "query: " << BeatBoard::ApiCommon::escape(query) << std::endl;

  std::string memcached_host = "127.0.0.1";
  int memcached_port = 11211;

  BeatBoard::ApiMemcached* memcached = new BeatBoard::ApiMemcached(memcached_host, memcached_port);
  std::cerr << memcached->memcachedStatus() << std::endl;

  std::string key = "dummy";
  memcached->setMemcachedData(key, key);

  std::string result;
  memcached->getMemcachedData(key, result);
  std::cerr << result << std::endl;

  return 0;
}
