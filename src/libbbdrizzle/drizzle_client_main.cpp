#include "drizzle_client.h"

int main( int argc, char** argv ) {
  std::string db = "test";
  std::string host = "localhost";
  in_port_t port = 8888;

  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " [port]" << std::endl;
  }
  else
  {
    port = atoi(argv[1]);
    std::cout << port << std::endl;
  }

  BeatBoard::DrizzleClient* client = new BeatBoard::DrizzleClient( host, port, db );

  BeatBoard::DrizzleResponse response;
  bool ret = false;

  std::string table_name = "test";
  std::string insert_columns_and_source = "(text) VALUES (\"test\")";
  ret = client->insert(table_name, insert_columns_and_source, response);
  if (!ret) {
    std::cout << "insert failed" << std::endl;
    return false;
  }
  std::cout << "insert success" << std::endl;

  std::string select_list = "*";
  std::string from_clause = "test";
  std::string where_clause = "";
  ret = client->select(select_list, from_clause, where_clause, response);

  drizzle_field_t field;
  size_t offset;
  size_t size;
  size_t total;

  if (ret)
  {
    std::cout << "select success" << std::endl;

    if (response.ret == DRIZZLE_RETURN_OK)
    {
      std::cout << "drizzle return ok" << std::endl;
    }

    //ret = drizzle_column_skip(&response.result);

    while ( drizzle_row_read(&response.result, &response.ret) != 0 && response.ret == DRIZZLE_RETURN_OK )
    {
      std::cout << "start read" << std::endl;
      while (1)
      {
        field= drizzle_field_read(&response.result, &offset, &size, &total, &response.ret);
        if ( response.ret == DRIZZLE_RETURN_ROW_END)
        {
          std::cout << "row end" << std::endl;
          break;
        }
        else if ( response.ret != DRIZZLE_RETURN_OK)
        {
          std::cout << "row ng" << std::endl;
          std::cout << "drizzle_field_read: " << client->drizzle_client_error() << std::endl;
          return 1;
        }

        if (field == NULL)
          std::cout << "NULL" << std::endl;
        else
          printf("%.*s", (int)size, field);

        if (offset + size == total)
          printf("\t");
      }

      printf("\n");
    }
  }
  else
  {
    std::cout << "failure" << std::endl;
  }

  std::cout << "end" << std::endl;
  return 0;
}
