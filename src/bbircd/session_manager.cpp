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
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("colector notify received");
  map<string, vector<string> >::iterator map_it = arg->begin();

  while(map_it != arg->end()) {
    vector<string>::iterator vec_it = map_it->second.begin();

    while(vec_it != map_it->second.end()) {
      vector<string> *buff = &(*this->sessionBuffer)[map_it->first] ;
      buff->push_back(*vec_it);
      vec_it++;
    }

    vector<string> *buff = &(*this->sessionBuffer)[map_it->first] ;

    while(buff->size() > 10) {
      buff->erase(buff->begin());
    }

    *(this->notifiable) = true;
    map_it++;
  }

  return true;
}

static void notify_timer(int fd, short event, void *arg) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  BeatBoard::SessionManager *sessionManager = (BeatBoard::SessionManager*)arg;
  map<string, bool>::iterator notifiable = sessionManager->notifiableSessionId.begin();

  while(notifiable != sessionManager->notifiableSessionId.end()) {
    if(notifiable->second) {
      notifiable->second = false;
      logger.debug("notify timer: " + notifiable->first);

      BeatBoard::HTTPAPIReadNotifier* notifier = sessionManager->waitingConnections[notifiable->first];

      if(notifier != NULL) {
        notifier->notify(&(sessionManager->sessionBuffers[notifiable->first]));
        sessionManager->waitingConnections[notifiable->first] = NULL;
      } else {
        notifiable->second = true;
      }



    }

    notifiable++;
  }

  struct timeval tv;

  tv.tv_usec = 0;

  tv.tv_sec = 1;

  evtimer_add(&(sessionManager->timer), &tv);
}

BeatBoard::SessionManager::SessionManager() {
}

BeatBoard::SessionManager::~SessionManager() {

}

void BeatBoard::SessionManager::setUpTimer() {
  struct timeval tv;
  tv.tv_usec = 0;
  tv.tv_sec = 1;

  evtimer_set(&timer, notify_timer, this);
  evtimer_add(&timer, &tv);
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


void BeatBoard::SessionManager::destroyAllSession(){
  map<string, IRCConnection*>::iterator it = nick2ircConnection.begin();
  while(it != nick2ircConnection.end()){
    (*it).second->removeReadNotifiers();
  }
  return;
}

void BeatBoard::SessionManager::setHTTPAPIReadNotifier(string session_id,
    HTTPAPIReadNotifier* notifier) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("setNotifier" + session_id);

  if(this->waitingConnections[session_id] != NULL) {
    HTTPAPIReadNotifier* n = this->waitingConnections[session_id];
    this->waitingConnections[session_id] = NULL;
    n->timeout_response();
    delete(n);
  }

  if(this->notifiableSessionId[session_id]) {
    this->notifiableSessionId[session_id] = false;
    logger.debug("notify hooked by client connection:" + session_id);
    notifier->notify(&(this->sessionBuffers[session_id]));
    this->waitingConnections[session_id] = NULL;
  } else {
    this->waitingConnections[session_id] = notifier;
  }
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
