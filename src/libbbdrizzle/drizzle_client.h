// need libdrizzle http://launchpad.net/libdrizzle
// % g++ -G -o libbbdrizzle.so -dynamiclib -Kpic  drizzle_client.cc -ldrizzle

#ifndef DRIZZLE_CLIENT_H
#define DRIZZLE_CLIENT_H

#include <iostream>
#include <string>

#include <libdrizzle/drizzle_client.h>
#include "drizzle_response.h"
#include "api_common.h"

namespace BeatBoard {

  class DrizzleClient {
  private:
    drizzle_st drizzle;
    drizzle_con_st con;

    std::string db;
    std::string host;
    in_port_t port;
	unsigned int db_type;
	std::string user;
	std::string password;

  public:
    DrizzleClient( const std::string& host, const in_port_t port, const std::string& db, const unsigned int db_type, const std::string& user, const std::string& password );
    virtual ~DrizzleClient();
    bool insert( const std::string& table_name, const std::string& insert_columns_and_source, BeatBoard::DrizzleResponse& response );
    bool select( const std::string& select_list, const std::string& from_clause, const std::string& where_clause, BeatBoard::DrizzleResponse& response );
	bool update(  const std::string& target_table, const std::string& set_clause_list, const std::string& where_clause, BeatBoard::DrizzleResponse& response );
	std::string client_error();

  private:
    void connect();
    void send_query( const std::string& query, BeatBoard::DrizzleResponse& response );
    bool check_drizzle_response( BeatBoard::DrizzleResponse& response, const bool drizzle_column_skip_flag );
    void cleanup();
  };
}

#endif
