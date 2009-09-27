#ifndef QUERY_LOGGER_H
#define QUERY_LOGGER_H

#include <uuid/uuid.h>
#include <iostream>
#include <string>
#include "drizzle_client.h"

namespace BeatBoard {
  class QueryLogger {
  private:
    std::string table_name;
    BeatBoard::DrizzleClient* client;
    BeatBoard::DrizzleResponse response;

  public:
    QueryLogger( const std::string& tablename, 
                 BeatBoard::DrizzleClient* client );
    virtual ~QueryLogger();
    bool insertQueryLogToDB( const std::string& query );
  };
}

#endif
