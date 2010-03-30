#include <stdio.h>
#include "irc_connection.h"
#include <signal.h>
using namespace std;

void* func(void* args);
void sigcatch(int sig);
BeatBoard::IRCConnection *conn;
string channel;

static void fifo_read(int fd, short event, void *arg) {
  //    struct event *ev = arg;
  char buf[1024];
  memset(buf, 0, 1024);
  read(fd, buf, 1024);
  conn->PRIVMSG(channel, buf);
}

BeatBoard::IRCConnection* setupConnection() {
  char buf[1024];
  //cout << "nick: ";
  //cin.getline(buf,1024);
  string nick(buf);
  nick = string("yushi_im_");

  //cout << "pass: ";
  //cin.getline(buf, 1024);
  string pass(buf);
  pass = string("guestdegues");


  //cout << "server: ";
  //cin.getline(buf, 1024);
  string server(buf);
  server = string("bb.isasaka.net");
  BeatBoard::IRCConnection *ret = new BeatBoard::IRCConnection(nick, &pass);
  ret->connectIRCServer(server, string("6667"));
  return ret;
}

int main(void) {

  char buf[1024];
  string line;

  if (SIG_ERR == signal(SIGINT, sigcatch)) {
    fprintf(stderr, "failed to set signalhandler.\n");
    exit(1);
  }

  //cout << "channel: ";
  //cin.getline(buf, 1024);
  channel = string(buf);
  channel = string("#bb");

  conn = setupConnection();
  conn->JOIN(channel);

  struct event evfifo;
  event_set(&evfifo, 0, EV_READ | EV_PERSIST, fifo_read, &evfifo);
  event_add(&evfifo, NULL);

  BeatBoard::IRCConnection::bb_event_dispatch(NULL);
  return 0;
}

void sigcatch(int sig) {
  fprintf(stdout, "catch signal %d\n", sig);
  //if(conn != NULL){
  //delete conn;
  //}
  //BeatBoard::IRCConnection::bb_event_finish();
  exit(0);
}
