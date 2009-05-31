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
  std::string userinfo = ApiCommon::escape(request->userinfo());
  std::string result;
  
  std::cout << "type: " << type << std::endl;
  std::cout << "username: " << username << std::endl;
  std::cout << "password: " << password << std::endl;

  if (type == AUTHAPI_ADD_USER)
  {
      std::cout << "add user" << std::endl;
      bool ret = insertAccountToDB( username, password, userinfo, result );
      if (ret)
      {
        response->set_result(AUTHAPI_ACCOUNT_INSERT_OK);
      }
      else
      {
        response->set_result(AUTHAPI_ACCOUNT_EXIST);
      }
      response->set_error(result);
  }
  else if (type == AUTHAPI_VERIFY_USER)
  {
      std::cout << "verify user account" << std::endl;
      bool ret = verifyAccountFromDB( username, password, result );
      if (ret)
      {
        response->set_result(AUTHAPI_VERIFY_OK);
      }
      else 
      {
        response->set_result(AUTHAPI_VERIFY_ERROR);
      }
      response->set_error(result);
  }
  else if (type == AUTHAPI_UPDATE_USERINFO)
  {
      std::cout << "update user info" << std::endl;

      bool ret = updateUserInfoToDB( username, password, userinfo, result );
      if (ret)
      {
        response->set_result(AUTHAPI_UPDATE_OK);
      }
      else 
      {
        response->set_result(AUTHAPI_UPDATE_ERROR);
      }
      response->set_error(result);
  }

  done->Run();
}

bool
BeatBoard::AuthApiService::updateUserInfoToDB( const std::string& username, 
                                               const std::string& password,
                                               const std::string& userinfo,
                                               std::string& result )
{
  bool ret = false;

  std::string target_table = table_name;
  std::string set_clause_list = "userinfo = \'" + userinfo + "\'";
  std::string where_clause = "where username = \'" + username + "\' limit 1";

  ret = client->update(target_table, set_clause_list, where_clause, response);
  if (response.ret == DRIZZLE_RETURN_OK)
  {
    std::cerr << "drizzle return ok" << std::endl;
    result = "update success";
    ret = true;
  }
  else
  {
    std::cerr << "drizzle db error" << std::endl;
    result = "update failed";
  }

  return ret;
}


bool
BeatBoard::AuthApiService::verifyAccountFromDB( const std::string& username, 
                                                const std::string& password,
                                                std::string& result )
{
  bool ret = false;

  std::string select_list = "password";
  std::string from_clause = table_name;
  std::string where_clause = "where username = \'" + username + "\' limit 1";

  ret = client->select(select_list, from_clause, where_clause, response);
  if (response.ret == DRIZZLE_RETURN_OK)
  {
    std::cerr << "drizzle return ok" << std::endl;
  }
  else
  {
    std::cerr << "drizzle db error" << std::endl;
    return ret;
  }

  if (drizzle_row_read(&response.result, &response.ret) != 0)
  {
    std::string _password;
    bool read_ret = getPasswordFromField(_password);
    if (read_ret)
    {
      std::cerr << _password << ":" << password << std::endl;
      std::cerr << _password.size() << ":" << password.size() << std::endl;
      std::cerr << (password == _password) << std::endl;
      if (password == _password)
      {
        result = "password correct";
        std::cerr << result << std::endl;
        ret = true;
      }
      else
      {
        result = "password incorrect";
        std::cerr << result << std::endl;
        ret = false;
      }
    }
    else
    {
      ret = false;
    }
  }
  else
  {
    std::cerr << "account not exist" << std::endl;
    ret = false;
  }
  
  return ret;
}

bool
BeatBoard::AuthApiService::getPasswordFromField(std::string& password)
{
  drizzle_field_t field;
  size_t offset;
  size_t size;
  size_t total;

  while ( drizzle_row_read(&response.result, &response.ret) != 0 && response.ret == DRIZZLE_RETURN_OK )
  {
    while (1)
    {
      field= drizzle_field_read(&response.result, &offset, &size, &total, &response.ret);
      if (response.ret == DRIZZLE_RETURN_ROW_END)
      {
        std::cout << "row end" << std::endl;
        break;
      }
      else if (response.ret != DRIZZLE_RETURN_OK)
      {
        std::cout << "row ng" << std::endl;
        std::cout << "drizzle_field_read: " << client->client_error() << std::endl;
        return false;
      }

      if (field != NULL)
      {
        password = std::string(field);
        password.erase(password.size() - 1, 1); // chop!
      }
    }
  }
  return true;
}

bool
BeatBoard::AuthApiService::insertAccountToDB( const std::string& username, 
                                              const std::string& password,
                                              const std::string& userinfo,
                                              std::string& result )
{
  bool ret = false;

  if (!checkAccountExist(username))
  {
    uuid_t out;
    char uuidstr[UUID_UNPARSED_SIZE];

    uuid_generate(out);
    uuid_unparse(out, uuidstr);

    std::string insert_columns_and_source;

    insert_columns_and_source = "values (0,\'" + std::string(uuidstr) +
      "\', null, \'" + username + "\', \'" + password + "\', \'" + userinfo + "\' )";
    std::cout << insert_columns_and_source << std::endl;
    ret = client->insert(table_name, insert_columns_and_source, response);
    if (!ret)
    {
      result = "insert account failed";
      ret = false;
    }
    else
    {
      result = "insert success";
    }
  }
  else
  {
    result = "account already exist";
    ret = false;
  }

  return ret;
}

bool
BeatBoard::AuthApiService::checkAccountExist( const std::string& username )
{
  bool ret = false;

  std::string select_list = "*";
  std::string from_clause = table_name;
  std::string where_clause = "where username = \'" + username + "\' limit 1";

  ret = client->select(select_list, from_clause, where_clause, response);
  if (response.ret == DRIZZLE_RETURN_OK)
  {
    std::cerr << "drizzle return ok" << std::endl;
  }
  else
  {
    std::cerr << "drizzle db error" << std::endl;
    return ret;
  }
    //ret = drizzle_column_skip(&response.result);
  if (drizzle_row_read(&response.result, &response.ret) != 0)
  {
    std::cerr << "account already exist" << std::endl;
    readDrizzleRow();
    ret = true;
  }
  else
  {
    std::cerr << "account not exist" << std::endl;
    ret = false;
  }

  return ret;
}

void
BeatBoard::AuthApiService::readDrizzleRow()
{
  drizzle_field_t field;
  size_t offset;
  size_t size;
  size_t total;

  while ( drizzle_row_read(&response.result, &response.ret) != 0 && response.ret == DRIZZLE_RETURN_OK )
  {
    std::cout << "start read" << std::endl;
    while (1)
    {
      field= drizzle_field_read(&response.result, &offset, &size, &total, &response.ret);
      if (response.ret == DRIZZLE_RETURN_ROW_END)
      {
        std::cout << "row end" << std::endl;
        break;
      }
      else if (response.ret != DRIZZLE_RETURN_OK)
      {
        std::cout << "row ng" << std::endl;
        std::cout << "drizzle_field_read: " << client->client_error() << std::endl;
        return;
      }
    }
  }
  //ret = client->select(select_list, from_clause, where_clause, response);
}
