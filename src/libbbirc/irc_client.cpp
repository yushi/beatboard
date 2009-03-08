#include <pthread.h>
#include <stdio.h>
#include "irc_connection.h"
#include <signal.h>
using namespace std;

void* func(void* args);
void sigcatch(int sig);
BeatBoard::IRCConnection *conn;
string channel;
pthread_t pt;

int main(void){

  char buf[1024];
  string line;

  if(SIG_ERR == signal(SIGINT,sigcatch)){
    fprintf(stderr,"failed to set signalhandler.\n");
    exit(1);
  }

  cout << "nick: ";

  cin.getline(buf,1024);
  conn = new BeatBoard::IRCConnection(string(buf));

  cout << "server: ";
  cin.getline(buf, 1024);
  conn->connectIRCServer(string(buf), string("6667"));

  cout << "channel: ";
  cin.getline(buf, 1024);
  channel = string(buf);
  conn->JOIN(channel);

  pthread_create( &pt, NULL, &func, NULL );
  BeatBoard::IRCConnection::bb_event_dispatch();
  pthread_join( pt, NULL );
  return 0;
}

void sigcatch(int sig)
{
  fprintf(stdout,"catch signal %d\n",sig);
  if(conn != NULL){
    delete conn;
  }
  BeatBoard::IRCConnection::bb_event_finish();
  //pthread_detach(pt);
  //pthread_exit((void*)pt);
  exit(0);
  //signal(sig,SIG_DFL);
}

void* func(void* args){
  string line;

  while(getline(cin, line)){
    conn->PRIVMSG(channel, line);
    sleep(2);
  }
  return NULL;
}
