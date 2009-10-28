#include "../api_common.h"
#include "../api_memcached.h"

#include <gtest/gtest.h>

TEST(ApiCommonTest, Escape) {
  std::string query = "h,o\"g'e h,o\"g'e";
  std::string escaped_query = BeatBoard::ApiCommon::escape(query);
  ASSERT_EQ(std::string("h\\,o\\\"g\\'e h\\,o\\\"g\\\'e"), escaped_query);
}

TEST(ApiCommonTest, MemcacheSuccess) {
  std::string memcached_host = "127.0.0.1";
  int memcached_port = 11211;

  BeatBoard::ApiMemcached* memcached = new BeatBoard::ApiMemcached(memcached_host, memcached_port);
  EXPECT_TRUE(memcached->memcachedStatus());

  std::string key = "dummy";
  EXPECT_TRUE(memcached->setMemcachedData(key, key));

  std::string result;
  EXPECT_TRUE(memcached->getMemcachedData(key, result));
  EXPECT_EQ("dummy", result);
  delete memcached;
  memcached = NULL;
}

TEST(ApiCommonTest, MemcacheFailed) {
  std::string memcached_host = "127.0.0.1";
  int memcached_port = 11212;

  BeatBoard::ApiMemcached* memcached = new BeatBoard::ApiMemcached(memcached_host, memcached_port);
  EXPECT_TRUE(memcached->memcachedStatus());

  std::string key = "dummy";
  EXPECT_FALSE(memcached->setMemcachedData(key, key));

  std::string result;
  EXPECT_FALSE(memcached->getMemcachedData(key, result));
  delete memcached;
  memcached = NULL;
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
