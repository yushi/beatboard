#include "searchapi_service.h"

BeatBoard::SearchApiService::SearchApiService( const std::string& db, 
                                         const std::string& table_name, 
                                         const std::string& host, 
                                         const in_port_t port )
{
  client = new BeatBoard::DrizzleClient( host, port, db );
  this->table_name = table_name;
}

BeatBoard::SearchApiService::~SearchApiService()
{
  delete client;
  client = NULL;
}

void
BeatBoard::SearchApiService::RpcFunc(google::protobuf::RpcController* controller,
                                     const searchapi::Request* request,
                                     searchapi::Response* response,
                                     google::protobuf::Closure* done)
{
  std::cout << __func__ << std::endl;

  std::string query = request->query();
  std::string result = "";

  bool ret = searchDB( query, result );
  if (ret)
  {
    response->set_result(result);
    std::cout << "OK: " << response->result() << std::endl;
  }
  else 
  {
    response->set_result(result);
    response->set_error("message wasn't set");
    std::cout << "NG: " << response->result() << std::endl;
  }
  done->Run();
}

bool
BeatBoard::SearchApiService::searchDB( std::string& query, std::string& result )
{
  bool ret = false;

  std::string select_list = "*";
  std::string from_clause = table_name;
  std::string where_clause = "where message like '%" + query + "%'";

  ret = client->select(select_list, from_clause, where_clause, drizzle_response);
  
  if (ret && drizzle_response.ret == DRIZZLE_RETURN_OK )
  {
    ret = drizzleResultToJson(result);
  }
  else
  {
    std::cerr << "select failure" << std::endl;
  }

  return ret;
}

bool
BeatBoard::SearchApiService::drizzleResultToJson( std::string& result )
{
  bool ret = false;
  struct json_object *my_object = json_object_new_object();

  while ( drizzle_row_read(&drizzle_response.result, &drizzle_response.ret) != 0 && drizzle_response.ret == DRIZZLE_RETURN_OK )
  {
    ret = readDrizzleField( my_object );
  }

  result = std::string(json_object_to_json_string(my_object));
  return ret;
}


bool
BeatBoard::SearchApiService::readDrizzleField( struct json_object* my_object )
{
  drizzle_field_t field;
  size_t offset;
  size_t size;
  size_t total;
  std::string data;
  std::vector<std::string> field_data;

  while (1)
  {
    field= drizzle_field_read(&drizzle_response.result, &offset, &size, &total, &drizzle_response.ret);
    if ( drizzle_response.ret == DRIZZLE_RETURN_ROW_END)
    {
      std::cerr << "row end" << std::endl;
      fieldToJsonArray( my_object, field_data );
      field_data.clear();
      break;
    }
    else if ( drizzle_response.ret != DRIZZLE_RETURN_OK)
    {
      std::cerr << "drizzle_field_read: " << client->drizzle_client_error() << std::endl;
      return false;
    }

    if (field != NULL)
    {
      data = std::string(field);
      field_data.push_back(data.substr(0, size));
    }
  }
  return true;
}

void
BeatBoard::SearchApiService::fieldToJsonArray( struct json_object* my_object,
                                               std::vector<std::string>& field_data )
{
  struct json_object *my_array;
  my_array = json_object_new_array();

  std::vector<std::string>::iterator it = field_data.begin();
  it += 2; // skip added_id, id

  std::string ts = *it;
  it++;

  char *c_ts;
  c_ts = (char *)malloc(sizeof(char) * ts.size() + 1);
  std::strncpy(c_ts, ts.c_str(), ts.size() + 1);
  
  for( ; it != field_data.end(); ++it )
  {
    std::cout << *it << std::endl;
    char *c_char;
    c_char = (char *)malloc(sizeof(char) * it->size() + 1);
    std::strncpy(c_char, it->c_str(), it->size() + 1);
    json_object_array_add(my_array, json_object_new_string(c_char));
    free(c_char);
  }
  json_object_object_add(my_object, c_ts, my_array);
  free(c_ts);
}
