#include "query_logger.h"

static const unsigned int UUID_UNPARSED_SIZE = 32;

BeatBoard::QueryLogger::QueryLogger( const std::string& table_name,
                                     BeatBoard::DrizzleClient* client )
{
  this->table_name = table_name;
  this->client = client;
}

BeatBoard::QueryLogger::~QueryLogger()
{
}

bool
BeatBoard::QueryLogger::insertQueryLogToDB( const std::string& query )
{
  bool ret = false;
  uuid_t out;
  char uuidstr[UUID_UNPARSED_SIZE];

  uuid_generate(out);
  uuid_unparse(out, uuidstr);

  std::string insert_columns_and_source;
  insert_columns_and_source = "values (null,\'" + std::string(uuidstr) +
    "\', null, \'" + query + "\')";

  std::cout << insert_columns_and_source << std::endl;
  ret = client->insert(table_name, insert_columns_and_source, response);
  return ret;
}
