#ifndef SEARCHAPI_SERVICE_H
#define SEARCHAPI_SERVICE_H

#include <iostream>
#include <string>

#include <vector>
#include <fstream>

#include "searchapi.pb.h"
#include "searchapi_service_common.h"
#include "drizzle_client.h"

#include <json/json.h>


namespace BeatBoard {
  class SearchApiService : public searchapi::RpcService {
  private:
	BeatBoard::DrizzleClient* client;
    BeatBoard::DrizzleResponse drizzle_response;
    std::string table_name;

    bool searchDB( std::string& query, std::string& result );
    bool drizzleResultToJson( std::string& result );
    bool readDrizzleField( struct json_object* my_object );
    void fieldToJsonArray( struct json_object* my_object,
                           std::vector<std::string>& field_data );

  public:
	SearchApiService( const std::string& db, const std::string& table_name, 
                   const std::string& host, const in_port_t port );
	virtual ~SearchApiService();
    void RpcFunc( google::protobuf::RpcController* controller,
                  const searchapi::Request* request,
                  searchapi::Response* response,
                  google::protobuf::Closure* done );
  };
}

#endif
