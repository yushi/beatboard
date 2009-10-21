#include "../irc_channel.h"
#include <gtest/gtest.h>
using namespace BeatBoard;
namespace {
  class IRCChannelTest : public testing::Test {
  protected:
    IRCChannel* target;
    virtual void SetUp() {
      this->target = new IRCChannel();
    }
    
    virtual void TearDown() {
      if(this->target){
        delete this->target;
      }
    }
  };

  TEST_F(IRCChannelTest, add_and_getMessage){
    this->target->addMessage(string("user"), string("message"));
    vector<string> ret = this->target->getMessages();
    ASSERT_EQ((unsigned)2, ret.size());
    ASSERT_EQ(string("user"), ret[0]);
    ASSERT_EQ(string("message"), ret[1]);

    ret = this->target->getMessages();
    ASSERT_EQ((unsigned)0, ret.size());    

    this->target->addMessage(string("userA"), string("messageA"));
    this->target->addMessage(string("userB"), string("messageB"));
    ret = this->target->getMessages();
    ASSERT_EQ((unsigned)4, ret.size());
    ASSERT_EQ(string("userA"), ret[0]);
    ASSERT_EQ(string("messageA"), ret[1]);
    ASSERT_EQ(string("userB"), ret[2]);
    ASSERT_EQ(string("messageB"), ret[3]);

    this->target->addMessage(string("userA"), string("messageA"), 1);
    this->target->addMessage(string("userB"), string("messageB"), 1);
    ret = this->target->getMessages();
    ASSERT_EQ((unsigned)0, ret.size());
  }

  TEST_F(IRCChannelTest, addUsers){
    vector<string> users = this->target->getUsers();
    ASSERT_EQ((unsigned)0, users.size());

    this->target->addUser("userA");
    this->target->addUser("userB");
    this->target->addUser("userC");
    this->target->addUser("userD");
    this->target->addUserEnd();

    users = this->target->getUsers();
    ASSERT_EQ((unsigned)4, users.size());
    ASSERT_EQ(string("userA"), users[0]);
    ASSERT_EQ(string("userB"), users[1]);
    ASSERT_EQ(string("userC"), users[2]);
    ASSERT_EQ(string("userD"), users[3]);


    this->target->addUser("userC");
    this->target->addUser("userB");
    this->target->addUser("userA");
    this->target->addUserEnd();

    users = this->target->getUsers();
    ASSERT_EQ((unsigned)3, users.size());
    ASSERT_EQ(string("userC"), users[0]);
    ASSERT_EQ(string("userB"), users[1]);
    ASSERT_EQ(string("userA"), users[2]);



  }

  TEST_F(IRCChannelTest, add_delUser){
    vector<string> users = this->target->getUsers();
    ASSERT_EQ((unsigned)0, users.size());

    this->target->addUserJoin(string("usernameA!ipaddr"));
    users = this->target->getUsers();
    ASSERT_EQ((unsigned)1, users.size());
    ASSERT_EQ(string("usernameA"), users[0]);

    this->target->addUserJoin(string("usernameB!ipaddr"));
    users = this->target->getUsers();
    ASSERT_EQ((unsigned)2, users.size());
    ASSERT_EQ(string("usernameA"), users[0]);
    ASSERT_EQ(string("usernameB"), users[1]);

    this->target->addUserJoin(string("@usernameC!ipaddr"));
    users = this->target->getUsers();
    ASSERT_EQ((unsigned)3, users.size());
    ASSERT_EQ(string("usernameA"), users[0]);
    ASSERT_EQ(string("usernameB"), users[1]);
    ASSERT_EQ(string("@usernameC"), users[2]);

    this->target->delUser("usernameA");
    users = this->target->getUsers();
    ASSERT_EQ((unsigned)2, users.size());
    ASSERT_EQ(string("usernameB"), users[0]);
    ASSERT_EQ(string("@usernameC"), users[2]);

    this->target->delUser("usernameB!hogehoge");
    users = this->target->getUsers();
    ASSERT_EQ((unsigned)1, users.size());
    ASSERT_EQ(string("@usernameC"), users[2]);

    this->target->delUser("usernameC");
    users = this->target->getUsers();
    ASSERT_EQ((unsigned)0, users.size());
  }

  TEST_F(IRCChannelTest, hasMessage){
    ASSERT_FALSE(this->target->hasMessage());

    this->target->addMessage(string("user"), string("message"));
    ASSERT_TRUE(this->target->hasMessage());

    this->target->getMessages();
    ASSERT_FALSE(this->target->hasMessage());
  }

}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
