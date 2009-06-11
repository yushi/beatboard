#include "api_memcached.h"

BeatBoard::ApiMemcached::ApiMemcached( const std::string& memcached_host, 
                                       const int memcached_port )
{
  this->expiration = 60;
  setUpMemcached( memcached_host, memcached_port );
}

BeatBoard::ApiMemcached::~ApiMemcached()
{
  memcached_free(memc);
}

bool
BeatBoard::ApiMemcached::memcachedStatus()
{
  if (memcached_status)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void
BeatBoard::ApiMemcached::setUpMemcached( const std::string& memcached_host,
                                         const int memcached_port )
{
  memcached_server_st *servers;
  memcached_return rc;

  memcached_status = false;
  memc = memcached_create(NULL);
  servers = memcached_server_list_append(NULL, memcached_host.c_str(),
                                         memcached_port, &rc);
  if (rc != MEMCACHED_SUCCESS) {
    std::cerr << memcached_strerror(memc, rc) << std::endl;
    return;
  }

  rc = memcached_server_push(memc, servers);
  if (rc != MEMCACHED_SUCCESS) {
    std::cerr << memcached_strerror(memc, rc) << std::endl;
    return;
  }

  memcached_server_list_free(servers);
  memcached_status = true;
}

bool
BeatBoard::ApiMemcached::setMemcachedData( std::string& key, std::string& result )
{
  bool ret = true;

  std::cerr << __func__ << std::endl;

  if (memcached_status == false)
  {
    return false;
  }

  memcached_return rc;
  rc = memcached_set( memc, key.c_str(), key.size(),
                      result.c_str(), result.size(),
                      this->expiration, (uint32_t)0 );
  if (rc != MEMCACHED_SUCCESS && rc != MEMCACHED_BUFFERED)
  {
    std::cerr << memcached_strerror(memc, rc) << std::endl;
    ret = false;
  }
  return ret;
}

bool
BeatBoard::ApiMemcached::getMemcachedData( std::string& key, std::string& result )
{
  bool ret = true;
  char *value;
  size_t value_length;
  uint32_t flags;

  std::cerr << __func__ << std::endl;

  if (memcached_status == false)
  {
    return false;
  }

  memcached_return rc;
  value = memcached_get( memc, key.c_str(), key.size(),
                         &value_length, &flags, &rc);
  if (rc != MEMCACHED_SUCCESS)
  {
    std::cerr << memcached_strerror(memc, rc) << std::endl;
    ret = false;
  }
  else
  {
    std::cerr << "memc value: " << std::string(value) << std::endl;
    result = std::string(value);
  }
  return ret;
}

