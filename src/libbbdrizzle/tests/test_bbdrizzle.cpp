#include "../drizzle_client.h"
#include "api_common.h"

#include <gtest/gtest.h>

TEST(BBDrizzleTest, SQLInsertFailed) {
  BeatBoard::DrizzleClient* client;
  BeatBoard::DrizzleResponse response;

  std::string db = "test";
  std::string table = "test_dummy";
  std::string host = "127.0.0.1";
  in_port_t port = 3306;
  std::string user = "user";
  std::string password = "";

  client = new BeatBoard::DrizzleClient( host, port, db, BeatBoard::DB_MYSQL, user, password );

  std::string insert_columns_and_source = "(text) VALUES (\"test\")";
  bool ret = client->insert(table, insert_columns_and_source, response);
  EXPECT_FALSE(ret);

  delete client;
  client = NULL;
}

TEST(BbDrizzleTest, SQLinsertSuccess) {
  BeatBoard::DrizzleClient* client;
  BeatBoard::DrizzleResponse response;

  std::string db = "test";
  std::string table = "test1";
  std::string host = "127.0.0.1";
  in_port_t port = 3306;
  std::string user = "user";
  std::string password = "";

  client = new BeatBoard::DrizzleClient( host, port, db, BeatBoard::DB_MYSQL, user, password );

  // create table test1 ( testID INTEGER NOT NULL AUTO_INCREMENT PRIMARY KEY, text TEXT);
  std::string insert_columns_and_source = "(text) VALUES (\"test\")";
  bool ret = client->insert(table, insert_columns_and_source, response);
  EXPECT_TRUE(ret);

  delete client;
  client = NULL;
}

TEST(BbDrizzleTest, SQLselect) {
  BeatBoard::DrizzleClient* client;
  BeatBoard::DrizzleResponse response;

  std::string db = "test";
  std::string table = "test1";
  std::string host = "127.0.0.1";
  in_port_t port = 3306;
  std::string user = "user";
  std::string password = "";

  client = new BeatBoard::DrizzleClient( host, port, db, BeatBoard::DB_MYSQL, user, password );

  std::string select_list = "*";
  std::string from_clause = table;
  std::string where_clause = "where text like '%test%'";

  bool ret = client->select(select_list, from_clause, where_clause, response);
  EXPECT_TRUE(ret);

  delete client;
  client = NULL;
}

TEST(BbDrizzleTest, SQLupdate) {
  BeatBoard::DrizzleClient* client;
  BeatBoard::DrizzleResponse response;

  std::string db = "test";
  std::string table = "test1";
  std::string host = "127.0.0.1";
  in_port_t port = 3306;
  std::string user = "user";
  std::string password = "";

  client = new BeatBoard::DrizzleClient( host, port, db, BeatBoard::DB_MYSQL, user, password );

  std::string set_clause_list = "text = '12345'";
  std::string update_where_clause = "where text = 'test'";

  bool ret = client->update(table, set_clause_list, update_where_clause, response);
  EXPECT_TRUE(ret);

  delete client;
  client = NULL;
}



int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



