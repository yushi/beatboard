#include "http_api_notifier.h"

static void timeout_timer_function(int fd, short event, void *arg) {
  BeatBoard::HTTPAPIReadNotifier* instance = (BeatBoard::HTTPAPIReadNotifier*)arg;
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("timeout time hooked");
  if (instance != NULL) {
    instance->timeout_timer_enabled = false;
    instance->timeout_response();
  }
}

BeatBoard::HTTPAPIReadNotifier::HTTPAPIReadNotifier(struct evhttp_request *req, int timeout) {
  this->timeout = timeout;
  this->req = req;
  this->init_time = time(NULL);
  this->buf = NULL;
  this->isResponsed = false;
  // set timeout callback
  struct timeval tv;
  tv.tv_usec = 0;
  tv.tv_sec = timeout;

  evtimer_set(&(this->timeout_timer), timeout_timer_function, this);
  evtimer_add(&(this->timeout_timer), &tv);
  this->timeout_timer_enabled = true;
}

BeatBoard::HTTPAPIReadNotifier::~HTTPAPIReadNotifier() {
}

void BeatBoard::HTTPAPIReadNotifier::timeout_response(){
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("timeout exceeded");

  if(this->timeout_timer_enabled){
    evtimer_del(&(this->timeout_timer));
    this->timeout_timer_enabled = false;
  }

  if (!(this->isResponsed)) {
    this->isResponsed = true;
    evhttp_send_error(this->req, 408, "Time out");
  }
}

bool BeatBoard::HTTPAPIReadNotifier::notify(map<string, vector<string> >* messages) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  if(this->isResponsed == true){
    logger.debug("notify called but already responsed");
    return false;
  }

  if(this->timeout_timer_enabled){
    evtimer_del(&(this->timeout_timer));    
    this->timeout_timer_enabled = false;
  }
  
  this->isResponsed = true;
  logger.debug("NOTIFY");

  if (this->req == NULL) {
    logger.debug("req == NULL, nothing to do");
    return false;
  }

  evhttp_request* req = this->req;
  this->req = NULL;

  struct evbuffer *buf;
  buf = evbuffer_new();

  // rollback for timeout (2:59)
  if ((time(NULL) - this->init_time) > (this->timeout) - 1) {
    logger.info("client was timeout. response already backed");
    logger.debug("NOTIFY canceled");
    return false;
  }

  if (buf == NULL) {
    logger.info("failed to create response buffer");
    logger.debug("NOTIFY canceled");
    evhttp_send_error(req, 500, "Internal Server Error");
    return false;
  }

  json_object *jsonobj = json_object_new_object();
  map<string, vector<string> >::iterator it = (*messages).begin();

  while (it != (*messages).end()) {
    string channel = ((*it).first).c_str();
    json_object *messages = json_object_new_array();

    for (unsigned int i = 0; i < (*it).second.size(); i += 2) {
      char* type = (char*)((*it).second[i]).c_str();
      char* message = (char*)((*it).second[i+1]).c_str();

      json_object_array_add(messages, json_object_new_string(type));
      json_object_array_add(messages, json_object_new_string(message));
    }

    it->second.clear();
    ++it;
    json_object_object_add(jsonobj, (char*)channel.c_str(), messages);
  }

  const char* json_str = json_object_get_string(jsonobj);
  free(jsonobj);
  size_t json_str_len = strlen(json_str);
  evhttp_add_header(req->output_headers,
                    "Content-type",
                    "application/x-javascript; charset=utf-8");

  ostringstream csize;
  csize << json_str_len;
  evhttp_add_header(req->output_headers, "Content-Length", csize.str().c_str());

  evbuffer_add(buf, json_str, json_str_len);
  evhttp_send_reply(req, HTTP_OK, "OK", buf);
  logger.debug(json_str);
  logger.debug("NOTIFY END");
  logger.debug("READ finished.");
  evbuffer_free(buf);
  return true;
}
