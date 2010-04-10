#include "session_manager.h"

BeatBoard::MessageCollector::MessageCollector(string session_id,
    map<string, vector<string> >* sessionBuffer,
    bool* notifiable) {
  this->session_id = session_id;
  this->sessionBuffer = sessionBuffer;
  this->isPersistent = true;
  this->notifiable = notifiable;
}

BeatBoard::MessageCollector::~MessageCollector() {
}


bool BeatBoard::MessageCollector::notify(map<string, vector<string> >* arg) {
  cout << "colector notify" << endl;
  map<string, vector<string> >::iterator map_it = arg->begin();

  while (map_it != arg->end()) {
    vector<string>::iterator vec_it = map_it->second.begin();

    while (vec_it != map_it->second.end()) {
      vector<string> *buff = &(*this->sessionBuffer)[map_it->first] ;
      buff->push_back(*vec_it);
      vec_it++;
    }

    *(this->notifiable) = true;
    map_it++;
  }

  return true;
}

static void notify_timer(int fd, short event, void *arg) {
  BeatBoard::SessionManager *sessionManager = (BeatBoard::SessionManager*)arg;
  printf("notify hook at SessionManager\n");

  map<string, bool>::iterator notifiable = sessionManager->notifiableSessionId.begin();

  while (notifiable != sessionManager->notifiableSessionId.end()) {
    if (notifiable->second) {
      notifiable->second = false;
      sessionManager->waitingConnections[notifiable->first]->notify(&(sessionManager->sessionBuffers[notifiable->first]));
    }

    notifiable++;
  }

  struct timeval tv;

  tv.tv_usec = 0;

  tv.tv_sec = 1;

  evtimer_add(&(sessionManager->timer), &tv);
}

BeatBoard::SessionManager::SessionManager() {
  struct timeval tv;
  tv.tv_usec = 0;
  tv.tv_sec = 1;

  evtimer_set(&timer, notify_timer, this);
  evtimer_add(&timer, &tv);
}

BeatBoard::SessionManager::~SessionManager() {

}

string BeatBoard::SessionManager::createSession(IRCConnection* conn) {
  string session_id = this->createSessionId();
  nick2ircConnection[conn->getNick()] = conn;
  sessionId2nick[session_id] = conn->getNick();
  MessageCollector* collector = new MessageCollector(session_id,
      &sessionBuffers[session_id],
      &(this->notifiableSessionId[session_id]));
  conn->setReadNotifier(collector);
  return session_id;
}

void BeatBoard::SessionManager::setHTTPAPIReadNotifier(string session_id,
    HTTPAPIReadNotifier* notifier) {
  this->waitingConnections[session_id] = notifier;
}

BeatBoard::IRCConnection* BeatBoard::SessionManager::getIRCConnectionBySessionId(string session_id) {
  return nick2ircConnection[sessionId2nick[session_id]];
}

BeatBoard::IRCConnection* BeatBoard::SessionManager::getIRCConnectionByNick(string nick) {
  return nick2ircConnection[nick];
}

bool BeatBoard::SessionManager::isExistNick(string nick) {
  return (nick2ircConnection[nick] != NULL) ? true : false;
}

string BeatBoard::SessionManager::createSessionId() {
  uuid_t uuid;
  char buff[1024];
  uuid_generate(uuid);
  uuid_unparse_upper(uuid, buff);
  return string(buff);
}

