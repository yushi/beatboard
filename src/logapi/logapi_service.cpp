#include "logapi_service.h"

static const unsigned int UUID_UNPARSED_SIZE = 32;

BeatBoard::LogApiService::LogApiService( const std::string& db, 
                                         const std::string& table_name, 
                                         const std::string& host, 
                                         const in_port_t port,
                                         const unsigned int db_type,
                                         const std::string& user,
                                         const std::string& password )
{
  client = new BeatBoard::DrizzleClient( host, port, db, db_type, user, password );
  this->table_name = table_name;
}

BeatBoard::LogApiService::~LogApiService()
{
  delete client;
  client = NULL;
}

void
BeatBoard::LogApiService::RpcFunc(google::protobuf::RpcController* controller,
                                  const logapi::Request* request,
                                  logapi::Response* response,
                                  google::protobuf::Closure* done)
{
  std::cout << __func__ << std::endl;

  std::string channel = ApiCommon::escape(request->channel());
  std::string time = ApiCommon::escape(request->time());
  std::string identifier = ApiCommon::escape(request->identifier());
  std::string message = ApiCommon::escape(request->message());
  
  bool ret = insertLogToDB( channel, time, identifier, message );
  if (ret)
  {
    response->set_result(LOGAPI_RESULT_OK);
    response->set_error("logapi rpc server: insert success");
    std::cout << "OK: " << response->result() << std::endl;
  }
  else 
  {
    response->set_result(LOGAPI_RESULT_ERROR);
    response->set_error("logapi rpc server: insert faild");
    std::cout << "NG: " << response->result() << std::endl;
  }
  done->Run();
}

bool
BeatBoard::LogApiService::insertLogToDB( std::string& channel, std::string& time, std::string& identifier, std::string& message )
{
  bool ret = false;
  uuid_t out;
  char uuidstr[UUID_UNPARSED_SIZE];

  uuid_generate(out);
  uuid_unparse(out, uuidstr);

  std::cout << "uuid: " << uuidstr << std::endl;
  std::cout << "channel: " << channel << std::endl;
  std::cout << "time: " << time << std::endl;
  std::cout << "identifier: " << identifier << std::endl;
  std::cout << "message: " << message << std::endl;


  std::string insert_columns_and_source;
  insert_columns_and_source = "values (null,\'" + std::string(uuidstr) +
    "\', null, \'" + channel + "\', \'" + time + "\', \'" + identifier +
    "\', \'" + message + "\')";

  std::cout << insert_columns_and_source << std::endl;

  ret = client->insert(table_name, insert_columns_and_source, response);
  return ret;
}
