#include "logapi_service.h"

static const unsigned int UUID_UNPARSED_SIZE = 32;
static const std::string comma = ",";
static const std::string doublequotation = "\"";
static const std::string singlequotation = "'";
static const std::string backslash = "\\";


void
BeatBoard::LogApiService::replaceEscapeChar( std::string& data, std::string& escapechar)
{
  int position = data.find(escapechar);
  while (position != -1)
  {
    data.replace( position, 1, backslash + escapechar, 0, escapechar.size() + backslash.size());
    position = data.find( ".", position + 1 );
    std::cout << "data: " << data << std::endl;
  }
}

std::string
BeatBoard::LogApiService::escape( const std::string& data )
{
  std::vector<std::string> escape_char;
  escape_char.push_back(backslash);
  escape_char.push_back(comma);
  escape_char.push_back(doublequotation);
  escape_char.push_back(singlequotation);

  std::string escaped_data = data;
  std::vector<std::string>::iterator it = escape_char.begin();
  for( ; it != escape_char.end(); ++it )
  {
    std::cout << *it << std::endl;
    replaceEscapeChar( escaped_data, *it );
  }
  return escaped_data;
}


BeatBoard::LogApiService::LogApiService( const std::string& db, 
                                         const std::string& table_name, 
                                         const std::string& host, 
                                         const in_port_t port )
{
  client = new BeatBoard::DrizzleClient( host, port, db );
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

  std::string channel = escape(request->channel());
  std::string time = escape(request->time());
  std::string identifier = escape(request->identifier());
  std::string message = escape(request->message());
  
  bool ret = insertLogToDB( channel, time, identifier, message );
  if (ret)
  {
    response->set_result(LOGAPI_RESULT_OK);
    std::cout << "OK: " << response->result() << std::endl;
  }
  else 
  {
    response->set_result(LOGAPI_RESULT_ERROR);
    response->set_error("message wasn't set");
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
  insert_columns_and_source = "values (0,\'" + std::string(uuidstr) +
    "\', null, \'" + channel + "\', \'" + time + "\', \'" + identifier +
    "\', \'" + message + "\')";

  std::cout << insert_columns_and_source << std::endl;

  ret = client->insert(table_name, insert_columns_and_source, response);
  return ret;
}
