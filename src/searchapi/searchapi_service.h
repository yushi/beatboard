#ifndef SEARCHAPI_SERVICE_H
#define SEARCHAPI_SERVICE_H

#include <iostream>
#include <string>
#include <cstring>

#include <vector>
#include <fstream>

#include "searchapi.pb.h"
#include "searchapi_service_common.h"
#include "drizzle_client.h"
#include "api_common.h"
#include "query_parser.h"
#include "query_logger.h"

#include <json/json.h>
#include <libmemcached/memcached.h>

namespace BeatBoard {
  class SearchApiService : public searchapi::RpcService {
  private:
	BeatBoard::DrizzleClient* client;
    BeatBoard::DrizzleResponse drizzle_response;
	BeatBoard::QueryLogger* logger;
	QueryParser* parser;
    std::string table_name;
	memcached_st *memc;
	bool memcached_status;
	time_t expiration;

    bool searchDB( std::string& query, std::string& result );
    bool drizzleResultToJson( std::string& result );
    bool readDrizzleField( struct json_object* my_array );
    void fieldToJsonArray( struct json_object* my_array,
                           std::vector<std::string>& field_data );
	bool searchDrizzleDB( Query* query, std::string& result );
	bool setMemcachedData( std::string& query, std::string& result );
	bool getMemcachedData( std::string& query, std::string& result );
	void setUpMemcached( const std::string& memcached_host, 
						 const in_port_t memcached_port );
	std::string generateSqlWhereClause( Query* rawquery );
	std::string dateClause( const std::string& date );
	std::string wordsClause( std::vector<std::string*> *words );
	void logQuery( const std::string& query );
	Query* parseQuery( std::string& rawquery );

  public:
	SearchApiService( const std::string& db, 
					  const std::string& table_name, 
					  const std::string& drizzle_host, 
					  const in_port_t drizzle_port,
					  const std::string& memcached_host, 
					  const in_port_t memcached_port,
					  const std::string& query_log_table_name,
					  const unsigned int db_type,
					  const std::string& user,
					  const std::string& password );
	virtual ~SearchApiService();
    void RpcFunc( google::protobuf::RpcController* controller,
                  const searchapi::Request* request,
                  searchapi::Response* response,
                  google::protobuf::Closure* done );
  };
}

#endif
