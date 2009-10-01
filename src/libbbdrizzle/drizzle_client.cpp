#include "drizzle_client.h"

BeatBoard::DrizzleClient::DrizzleClient( const std::string& host, const in_port_t port, const std::string& db, const unsigned int db_type, const std::string& user, const std::string& password )
{
  this->host = host;
  this->port = port;
  this->db = db;
  this->db_type = db_type;
  this->user = user;
  this->password = password;
  connect();
}

BeatBoard::DrizzleClient::~DrizzleClient()
{
  cleanup();
}

void 
BeatBoard::DrizzleClient::cleanup() 
{
  drizzle_con_free(&con);
  drizzle_free(&drizzle);
}

void
BeatBoard::DrizzleClient::connect()
{
  (void)drizzle_create(&drizzle);
  (void)drizzle_con_create(&drizzle, &con);
  drizzle_con_set_db(&con, db.c_str());
  drizzle_con_set_tcp(&con, host.c_str(), port);

  if (BeatBoard::DB_MYSQL == db_type)
  {
    drizzle_con_add_options(&con, DRIZZLE_CON_MYSQL);
    drizzle_con_set_auth(&con, this->user.c_str(), this->password.c_str());
  }
}

bool
BeatBoard::DrizzleClient::insert( const std::string& table_name, const std::string& insert_columns_and_source, BeatBoard::DrizzleResponse& response )
{
  std::string query = "INSERT INTO " + table_name + " " + insert_columns_and_source;
  send_query( query, response );
  std::cout << "issued query" << std::endl;
  return check_drizzle_response( response, false );
}

bool
BeatBoard::DrizzleClient::select(  const std::string& select_list, const std::string& from_clause, const std::string& where_clause, BeatBoard::DrizzleResponse& response )
{
  std::string query = "SELECT " + select_list + " FROM " + from_clause + " " + where_clause;
  send_query( query, response );
  return check_drizzle_response( response, true );
}

bool
BeatBoard::DrizzleClient::update(  const std::string& target_table, const std::string& set_clause_list, const std::string& where_clause, BeatBoard::DrizzleResponse& response )
{
  std::string query = "UPDATE " + target_table + " SET " + set_clause_list + " " + where_clause;
  send_query( query, response );
  return check_drizzle_response( response, false );
}

std::string
BeatBoard::DrizzleClient::client_error()
{
  return drizzle_error(&drizzle);
}

void
BeatBoard::DrizzleClient::send_query( const std::string& query, BeatBoard::DrizzleResponse& response )
{
  std::cout << "query: " << query << std::endl;
  (void)drizzle_query(&con, &response.result, query.c_str(), query.size(), &response.ret);
}

bool
BeatBoard::DrizzleClient::check_drizzle_response( BeatBoard::DrizzleResponse& response, const bool drizzle_column_skip_flag )
{
  if (response.ret != DRIZZLE_RETURN_OK)
  {
    std::cout << "drizzle_query:" << client_error() << std::endl;
    return false;
  }

  if (drizzle_result_error_code( &response.result ) != 0)
  {
    std::cout << drizzle_result_error_code(&response.result) << ":" << drizzle_result_error(&response.result) << std::endl;
    return false;
  }
  std::cout << "checked result error code" << std::endl;

  if (drizzle_column_skip_flag)
  {
    response.ret = drizzle_column_skip(&response.result);
    if (response.ret != DRIZZLE_RETURN_OK)
    {
      std::cout << "drizzle_column_skip:" << client_error() << std::endl;
      return false;
    }
  }

  return true;
}
