#ifndef API_MEMCACHED_H
#define API_MEMCACHED_H

#include <iostream>
#include <string>
#include <libmemcached/memcached.h>

namespace BeatBoard {
  class ApiMemcached {    
  private:
	memcached_st *memc;
	bool memcached_status;
	time_t expiration;

	void setUpMemcached( const std::string& memcached_host, 
						 const int memcached_port );

  public:
    ApiMemcached( const std::string& memcached_host,
                  const int memcached_port );
    ~ApiMemcached();

	bool setMemcachedData( std::string& key, std::string& result );
	bool getMemcachedData( std::string& key, std::string& result );
    bool memcachedStatus();
  };
}

#endif
