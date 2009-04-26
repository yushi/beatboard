#include "authapi_service.h"

static const unsigned int UUID_UNPARSED_SIZE = 32;

BeatBoard::AuthApiService::AuthApiService( const std::string& db, 
                                         const std::string& table_name, 
                                         const std::string& host, 
                                         const in_port_t port )
{
  client = new BeatBoard::DrizzleClient( host, port, db );
  this->table_name = table_name;
}

BeatBoard::AuthApiService::~AuthApiService()
{
  delete client;
  client = NULL;
}

void
BeatBoard::AuthApiService::RpcFunc(google::protobuf::RpcController* controller,
                                  const authapi::Request* request,
                                  authapi::Response* response,
                                  google::protobuf::Closure* done)
{
  std::cout << __func__ << std::endl;

  unsigned int type = request->type();
  std::string username = ApiCommon::escape(request->username());
  std::string password = ApiCommon::escape(request->password());
  
  std::cout << "type: " << type << std::endl;
  std::cout << "username: " << username << std::endl;
  std::cout << "password: " << password << std::endl;

  if (type == AUTHAPI_ADD_USER)
  {
      std::cout << "add user" << std::endl;
      bool ret = insertAccountToDB( username, password );
      if (ret)
      {
        response->set_result(AUTHAPI_ACCOUNT_INSERT_OK);
      }
      else 
      {
        response->set_result(AUTHAPI_ACCOUNT_EXIST);
        response->set_error("account name already set");
      }
  }
  else if (type == AUTHAPI_VERIFY_USER)
  {
      std::cout << "verify user account" << std::endl;
      bool ret = verifyAccountFromDB( username, password );
      if (ret)
      {
        response->set_result(AUTHAPI_VERIFY_OK);
      }
      else 
      {
        response->set_result(AUTHAPI_VERIFY_ERROR);
        response->set_error("login incorrect");
      }
  }

  done->Run();
}

bool
BeatBoard::AuthApiService::verifyAccountFromDB( std::string& username, std::string& password )
{
  return true;
}

bool
BeatBoard::AuthApiService::insertAccountToDB( std::string& username, std::string& password )
{
  bool ret = false;

//   uuid_t out;
//   char uuidstr[UUID_UNPARSED_SIZE];

//   uuid_generate(out);
//   uuid_unparse(out, uuidstr);

//   std::cout << "uuid: " << uuidstr << std::endl;
//   std::cout << "channel: " << channel << std::endl;
//   std::cout << "time: " << time << std::endl;
//   std::cout << "identifier: " << identifier << std::endl;
//   std::cout << "message: " << message << std::endl;


//   std::string insert_columns_and_source;
//   insert_columns_and_source = "values (0,\'" + std::string(uuidstr) +
//     "\', null, \'" + channel + "\', \'" + time + "\', \'" + identifier +
//     "\', \'" + message + "\')";

//   std::cout << insert_columns_and_source << std::endl;

//   ret = client->insert(table_name, insert_columns_and_source, response);
  return ret;
}
