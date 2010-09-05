#include "bbircd.h"

BeatBoard::BBLogger *logger;

void sig_handler(int sig)
{
  switch(sig) {
  case SIGHUP:
    logger->info("SIGHUP received");
    break;
  case SIGTERM:
    logger->info("SIGTERM received");
    exit(0);
    break;
  }
}
void sig_handler_SIGPIPE(int sig) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("SIGPIPE received");
}

void BeatBoard::BBIRCD::Daemon::setUp(char* addr, int port, int timeout) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  
  try {
    this->httpd = new HTTPAPIServer();
    this->httpd->setUp(addr, port, timeout);
    this->httpd->setUpTimer();
    
  } catch (BeatBoard::Exception& error) {
    logger.debug(error.message.data());
    std::cerr << "daemon start failed.\n";
  }

  return;
}

void BeatBoard::BBIRCD::Daemon::shutdown(){
  delete(this->httpd);
}

void BeatBoard::BBIRCD::Daemon::startService() {
  while (!IRCConnection::bb_event_dispatch(this->httpd->http_ev_base)) {
    fprintf(stderr, "start fail\n");
    sleep(1);
  }

  cout << "event loop exited" << endl;
}

void unexpected_exception_handler(void) {
  std::cout << "this is unexcepted Exception handler\n";
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("unknown exception throwed");
}

void daemonize(char* pidfile, char* rundir)
{
  int i,lfp;
  char str[10];
  if(getppid()==1) return; /* already a daemon */
  i=fork();
  if (i<0) exit(1); /* fork error */
  if (i>0) exit(0); /* parent exits */
  
  setsid(); /* obtain a new process group */
  for (i=getdtablesize();i>=0;--i) close(i); /* close all descriptors */
  i=open("/dev/null",O_RDWR); dup(i); dup(i); /* handle standart I/O */
  umask(027); /* set newly created file permissions */
  chdir(rundir); /* change running directory */

  if(pidfile != NULL){
    lfp=open(pidfile ,O_RDWR|O_CREAT,0640);
    if (lfp<0) exit(1); /* can not open */
    if (lockf(lfp,F_TLOCK,0)<0) exit(0); /* can not lock */
    
    /* first instance continues */
    sprintf(str,"%d\n",getpid());
    write(lfp,str,strlen(str)); /* record pid to lockfile */
    close(lfp);
  }
  
  signal(SIGCHLD,SIG_IGN); /* ignore child */
  signal(SIGTSTP,SIG_IGN); /* ignore tty signals */
  signal(SIGTTOU,SIG_IGN);
  signal(SIGTTIN,SIG_IGN);
  signal(SIGHUP,sig_handler); /* catch hangup signal */
  signal(SIGTERM,sig_handler); /* catch kill signal */
}

int main(int argc, char* argv[]) {
  bool is_daemon = false;
  int result;
  int port = 8000;
  int timeout = 180; // 3min
  char default_addr[] = "0.0.0.0";
  char default_rundir[] = "/";
  char default_logfile[] = "/usr/bb/var/log/bbircd.log";
  char* addr = default_addr;
  char* pidfile = NULL;
  char* rundir = default_rundir;
  char* logfile = default_logfile;
  
  while((result=getopt(argc,argv,"ha:dp:P:t:"))!=-1){
    switch(result){
    case 'a':
      addr = (char*)calloc(1, sizeof(char) * strlen(optarg) + 1);
      strncpy(addr,optarg,strlen(optarg));
      break;
    case 'd':
      rundir = (char*)calloc(1, sizeof(char) * strlen(optarg) + 1);
      strncpy(rundir,optarg,strlen(optarg));
      break;
    case 'D':
      is_daemon= true;
      break;
    case 'l':
      rundir = (char*)calloc(1, sizeof(char) * strlen(optarg) + 1);
      strncpy(rundir,optarg,strlen(optarg));
      break;
    case 'p':
      port = atoi(optarg);
      break;
    case 'P':
      pidfile = (char*)calloc(1, sizeof(char) * strlen(optarg) + 1);
      strncpy(pidfile,optarg,strlen(optarg));
      cout << "pid:" << pidfile << endl;
      break;
    case 't':
      timeout = atoi(optarg);
      break;
    case 'h':
      std::cout << "bbircd [-a <address>] [-p <port>]" << endl;
      exit(-1);
      break;
    }
  }

  if(is_daemon){
    daemonize(pidfile, rundir);
  }
  
  logger = &(BeatBoard::BBLogger::getInstance());
  logger->logfile = logfile;
  logger->info("bbircd started");
  std::set_unexpected(unexpected_exception_handler);

  signal(SIGPIPE , sig_handler_SIGPIPE);
  BeatBoard::BBIRCD::Daemon bbircd;
  bbircd.setUp(addr,port,timeout);
  bbircd.startService();
  bbircd.shutdown();
  return 0;
}
