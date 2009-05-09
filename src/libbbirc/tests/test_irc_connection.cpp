#include <gtest/gtest.h>
#include "../irc_connection.h"
using namespace BeatBoard;
void* func(void* args);

namespace {
  // The fixture for testing class Foo.
  class IRCProtoTest : public testing::Test {
  public:
    BeatBoard::IRCConnection *conn;
    char buf[1024];
    pthread_t pt;
  protected:
    // You can remove any or all of the following functions if its body
    // is empty.
    
    IRCProtoTest() {
      // You can do set-up work for neach test here.
    }
    
    virtual ~IRCProtoTest() {
      // You can do clean-up work that doesn't throw exceptions here.
    }
    
    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:
    
    virtual void SetUp() {
      // Code here will be called immediately after the constructor (right
      // before each test).
      this->conn = NULL;
      bzero(this->buf, 1024);
      pthread_create( &pt, NULL, &func, buf );

    }
    
    virtual void TearDown() {
      // Code here will be called immediately after each test (right
      // before the destructor).
    }
    
    // Objects declared here can be used by all tests in the test case for Foo.
  };
  
  // Tests that the Foo::Bar() method does Abc.
  TEST_F(IRCProtoTest, connect) {
    usleep(1000);
    this->conn = new BeatBoard::IRCConnection(string("tester"));
    this->conn->connectIRCServer(string("127.0.0.1"), string("6667"));
    BeatBoard::IRCConnection::bb_event_dispatch();
    pthread_join( this->pt, NULL );
    ASSERT_STREQ("NICK :tester\r\nUSER tester 0 * ::beatboard\r\n", this->buf);
    delete this->conn;
  }

  TEST_F(IRCProtoTest, pong) {
    usleep(1000);
    this->conn = new BeatBoard::IRCConnection(string("tester"));
    this->conn->connectIRCServer(string("127.0.0.1"), string("6667"));
    this->conn->PONG("hoge");
    BeatBoard::IRCConnection::bb_event_dispatch();
    pthread_join( this->pt, NULL );
    ASSERT_STREQ("NICK :tester\r\nUSER tester 0 * ::beatboard\r\nPONG :hoge\r\n", this->buf);
    delete this->conn;
  }

  TEST_F(IRCProtoTest, JOIN) {
    usleep(1000);
    this->conn = new BeatBoard::IRCConnection(string("tester"));
    this->conn->connectIRCServer(string("127.0.0.1"), string("6667"));
    this->conn->JOIN("hoge");
    BeatBoard::IRCConnection::bb_event_dispatch();
    pthread_join( this->pt, NULL );
    ASSERT_STREQ("NICK :tester\r\nUSER tester 0 * ::beatboard\r\nJOIN :hoge\r\n", this->buf);
    delete this->conn;    
  }
  
  TEST_F(IRCProtoTest, PRIVMSG) {
    usleep(1000);
    this->conn = new BeatBoard::IRCConnection(string("tester"));
    this->conn->connectIRCServer(string("127.0.0.1"), string("6667"));
    this->conn->PRIVMSG("#hoge","hoge");
    BeatBoard::IRCConnection::bb_event_dispatch();
    pthread_join( this->pt, NULL );
    ASSERT_STREQ("NICK :tester\r\nUSER tester 0 * ::beatboard\r\nPRIVMSG #hoge :hoge\r\n", this->buf);
    delete this->conn;        
  }

}  // namespace

void* func(void* args){
  struct sockaddr_in saddr;
  struct sockaddr_in caddr;
  int listen_fd;
  int client_fd;
  socklen_t len = sizeof(struct sockaddr_in);
  int rsize;
  int on = 1;
  
  char *buf = (char*)args;
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  bzero((char *)&saddr, sizeof(saddr));
  saddr.sin_family        = PF_INET;
  saddr.sin_addr.s_addr   = INADDR_ANY;
  saddr.sin_port          = htons(6667);
  setsockopt(listen_fd ,SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  if (bind(listen_fd, (struct sockaddr *)&saddr, len) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  if (listen(listen_fd, 1) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  
  if ((client_fd = accept(listen_fd, (struct sockaddr *)&caddr, &len)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  
  close(listen_fd);
  
  rsize = recv(client_fd, buf, 1024, 0);
  
  if (rsize == -1) {
    perror("recv");
    exit(EXIT_FAILURE);
  } else {
    write(client_fd, buf, rsize);
  }

  usleep(1);
  if ( close(client_fd) < 0) {
    perror("close");
    exit(EXIT_FAILURE);
  }
  BeatBoard::IRCConnection::bb_event_finish();
  return NULL;
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
