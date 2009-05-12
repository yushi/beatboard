#ifndef AUTHAPI_SERVICE_H
#define AUTHAPI_SERVICE_H

#include <uuid/uuid.h>

#include <iostream>

#include <fstream>
#include <string>
#include <vector>

#include "authapi.pb.h"
#include "authapi_service_common.h"
#include "drizzle_client.h"
#include "api_common.h"

namespace BeatBoard {
  class AuthApiService : public authapi::RpcService {
  private:
	BeatBoard::DrizzleClient* client;
    BeatBoard::DrizzleResponse response;
    std::string table_name;

    bool insertAccountToDB( const std::string& username, 
							const std::string& password, 
							std::string& result );
    bool verifyAccountFromDB( const std::string& username, 
							  const std::string& password,
							  std::string& result );
	bool updateUserInfoToDB( const std::string& username, 
							 const std::string& password,
							 const std::string& userinfo,
							 std::string& result );
	bool checkAccountExist( const std::string& username );
	void readDrizzleRow();
	bool getPasswordFromField(std::string& password);


  public:
	AuthApiService( const std::string& db, const std::string& table_name, 
                   const std::string& host, const in_port_t port );
	virtual ~AuthApiService();
    void RpcFunc( google::protobuf::RpcController* controller,
                  const authapi::Request* request,
                  authapi::Response* response,
                  google::protobuf::Closure* done );
  };
}

#endif
