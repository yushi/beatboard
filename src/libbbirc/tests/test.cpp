#include <gtest/gtest.h>
#include "../irc_proto.h"
using namespace BeatBoard;
namespace {
  // The fixture for testing class Foo.
  class IRCProtoTest : public testing::Test {
  protected:
    IRCEvent *ret;
    // You can remove any or all of the following functions if its body
    // is empty.
    
    IRCProtoTest() {
      // You can do set-up work for each test here.
    }
    
    virtual ~IRCProtoTest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    
    virtual void SetUp() {
      // Code here will be called immediately after the constructor (right
      // before each test).
      this->ret = NULL;
    }
    
    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
      if(this->ret){
        delete this->ret;
      }
    }
    
    // Objects declared here can be used by all tests in the test case for Foo.
  };
  
  // Tests that the Foo::Bar() method does Abc.
  TEST_F(IRCProtoTest, parse_ng_1) {
    this->ret = parse_irc_message("");
    ASSERT_EQ(this->ret, (IRCEvent*)NULL);

    this->ret = parse_irc_message("PRIVMSG");
    ASSERT_EQ(this->ret, (IRCEvent*)NULL);

    this->ret = parse_irc_message("PRIVMSG #channel text");
    ASSERT_EQ(this->ret, (IRCEvent*)NULL);

    this->ret = parse_irc_message("PRIVMSG #channel text\r");
    ASSERT_EQ(this->ret, (IRCEvent*)NULL);

    this->ret = parse_irc_message("PRIVMSG #channel text\n");
    ASSERT_EQ(this->ret, (IRCEvent*)NULL);

    this->ret = parse_irc_message(":1 PRIVMSG #channel text\n");
    ASSERT_EQ(this->ret, (IRCEvent*)NULL);
  }

  TEST_F(IRCProtoTest, parse_privmsg_1) {
    this->ret = parse_irc_message("PRIVMSG #channelA text\r\n");
    ASSERT_NE(this->ret, (IRCEvent*)NULL);
    ASSERT_EQ(this->ret->prefix, (std::string*)NULL);    
    ASSERT_STREQ(this->ret->command->c_str(), "PRIVMSG");
    ASSERT_EQ(this->ret->param_num, 2);
    ASSERT_STREQ(this->ret->params[0]->c_str(), "#channelA");
    ASSERT_STREQ(this->ret->params[1]->c_str(), "text");
    ASSERT_EQ(this->ret->params[2], (std::string*)NULL);
  }

  TEST_F(IRCProtoTest, parse_privmsg_2) {
    this->ret = parse_irc_message("PRIVMSG #channelB :text hoge\r\n");
    ASSERT_NE(this->ret, (IRCEvent*)NULL);
    ASSERT_EQ(this->ret->prefix, (std::string*)NULL);
    ASSERT_STREQ(this->ret->command->c_str(), "PRIVMSG");
    ASSERT_EQ(this->ret->param_num, 2);
    ASSERT_STREQ(this->ret->params[0]->c_str(), "#channelB");
    ASSERT_STREQ(this->ret->params[1]->c_str(), "text hoge");
    ASSERT_EQ(this->ret->params[2], (std::string*)NULL);
  }  

  TEST_F(IRCProtoTest, parse_privmsg_3) {
    this->ret = parse_irc_message(":example.com PRIVMSG #channelC text\r\n");
    ASSERT_NE(this->ret, (IRCEvent*)NULL);
    ASSERT_STREQ(this->ret->prefix->c_str(), "example.com");
    ASSERT_STREQ(this->ret->command->c_str(), "PRIVMSG");
    ASSERT_EQ(this->ret->param_num, 2);
    ASSERT_STREQ(this->ret->params[0]->c_str(), "#channelC");
    ASSERT_STREQ(this->ret->params[1]->c_str(), "text");
    ASSERT_EQ(this->ret->params[2], (std::string*)NULL);
  }  
}  // namespace

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
