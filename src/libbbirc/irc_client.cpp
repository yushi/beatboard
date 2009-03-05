#include <pthread.h>
#include <stdio.h>
#include "irc_connection.h"

void* func(void* args);
BeatBoard::IRCConnection *conn;
std::string channel;

int main(void){
  pthread_t pt;
  std::string line;

  std::cout << "nick: ";
  std::getline(std::cin, line);
  conn = new BeatBoard::IRCConnection(line.c_str());

  std::cout << "server: ";
  std::getline(std::cin, line);
  conn->connectIRCServer(line, std::string("6667"));

  std::cout << "channel: ";
  std::getline(std::cin, channel);
  conn->joinIRCChannel(channel);

  pthread_create( &pt, NULL, &func, NULL );
  BeatBoard::IRCConnection::bb_event_dispatch();
  pthread_join( pt, NULL );
  return 0;
}

void* func(void* args){
  std::string line;
  while(std::getline(std::cin, line)){
    conn->privMSG(channel, line);
    sleep(2);
  }
  return NULL;
}
