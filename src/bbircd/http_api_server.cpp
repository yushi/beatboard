#include "http_api_server.h"

using namespace std;
int BeatBoard::HTTPAPIServer::timeout = 180; // default

static void timeout_timer(int fd, short event, void *arg) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("timeout exceeded");
  if(arg != NULL){
    evhttp_send_error((struct evhttp_request*)arg, 408, "Time out");
  }
  
}

BeatBoard::HTTPAPIServer::HTTPAPIServer() {
  this->httpd = NULL;
  this->http_ev_base = NULL;
}

BeatBoard::HTTPAPIServer::~HTTPAPIServer() {
  if (NULL != this->httpd) {
    evhttp_free(httpd);
  }

  if (NULL != this->http_ev_base) {
    event_base_free(this->http_ev_base);
  }
}

void BeatBoard::HTTPAPIServer::setUp(char *addr, int port, int timeout) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  this->http_ev_base = event_init();
  this->timeout = timeout;

  httpd = evhttp_new(this->http_ev_base);
  evhttp_set_timeout(httpd, this->timeout);

  if (evhttp_bind_socket(this->httpd, addr, port) != 0) {};

  evhttp_set_cb(httpd, "/", HTTPAPIServer::rootHandler, NULL);

  evhttp_set_cb(httpd, "/CONNECT", HTTPAPIServer::connectHandler, this);

  evhttp_set_cb(httpd, "/JOIN", HTTPAPIServer::joinHandler, this);

  evhttp_set_cb(httpd, "/SPEAK", HTTPAPIServer::speakHandler, this);

  evhttp_set_cb(httpd, "/EXIT", HTTPAPIServer::exitHandler, this);

  evhttp_set_cb(httpd, "/READ", HTTPAPIServer::readHandler, this);

  logger.debug("setup finished");
}

void BeatBoard::HTTPAPIServer::rootHandler(struct evhttp_request *req, void *arg) {
  struct evbuffer *buf;
  buf = evbuffer_new();

  if (buf == NULL) {
    fprintf(stderr, "failed to create response buffer\n");
  } else {
    evbuffer_add_printf(buf, "This is BeatBoard API Server<br />\n");
    evbuffer_add_printf(buf,  "%s", req->uri);
    evbuffer_add_printf(buf, "<br />\n");

    // HTTPAPIServer Header Dump
    if (evhttp_find_header(req->input_headers, "User-Agent")) {
      evbuffer_add_printf(buf, "%s", (char*)evhttp_find_header(req->input_headers, "User-Agent"));
      evbuffer_add_printf(buf, "<br />\n");
    }

    if (EVHTTP_REQ_POST == req->type) {
      evbuffer_add_printf(buf, "POST<br />\n");
    } else if (EVHTTP_REQ_GET == req->type) {
      evbuffer_add_printf(buf, "GET<br />\n");
    } else if (EVHTTP_REQ_HEAD == req->type) {
      evbuffer_add_printf(buf, "HEAD<br />\n");
    } else {
      evbuffer_add_printf(buf, "Method is unknown<br />\n");
    }

    HTTPAPIServer *instance = (HTTPAPIServer*)(arg);
    map<string, string> get_parameters = instance->parseParameter(req);
    //map<string,string> post_parameters = instance->parsePostParameter(req);
    string hogehoge = get_parameters["hogehoge"];

    if (hogehoge.size() != 0) {
      evbuffer_add_printf(buf, "%s", (char*)(get_parameters["hogehoge"].c_str()));
      evbuffer_add_printf(buf, "<br />\n");
      //POST???
      //      evbuffer_add_printf( buf, req->input_buffer);
      //evbuffer_add_printf( buf, "<br />\n" );
    }

    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }

  evbuffer_free(buf);
}
map<string, string> BeatBoard::HTTPAPIServer::parseParameter(struct evhttp_request *req) {
  if (EVHTTP_REQ_POST == req->type) {
    return this->parsePostParameter(req);
  }

  //  }else if(EVHTTP_REQ_GET == req->type){
  return this->parseGetParameter(req);
  //  }
}

map<string, string> BeatBoard::HTTPAPIServer::parsePostParameter(struct evhttp_request *req) {
  map<string, string> ret;
  int content_length = atoi(evhttp_find_header(req->input_headers, "Content-Length"));
  struct evkeyvalq* params = (evkeyvalq *)calloc(1, sizeof(struct evkeyvalq));
  string dummy_url = string("http://localhost/?");
  char *body = (char*)calloc(1, sizeof(char) * content_length + 1);
  evbuffer_remove(req->input_buffer, (void*)body, content_length);

  //replace "+" to " " for www-form-urlencoded
  int i = 0;

  for (i = 0; i < content_length; i++) {
    if (*(body + i) == '+') {
      *(body + i) = ' ';
    }
  }

  dummy_url += string(body);

  evhttp_parse_query(dummy_url.c_str(), params);
  struct evkeyval *header = (evkeyval*)calloc(1, sizeof(struct evkeyval));
  TAILQ_FOREACH(header, params, next) {
    ret[string(header->key)] = string(header->value);
  }

  free(params);
  free(body);
  free(header);
  return ret;
}

map<string, string> BeatBoard::HTTPAPIServer::parseGetParameter(struct evhttp_request *req) {
  map<string, string> ret;
  const char* uri = req->uri;
  struct evkeyvalq* params = (evkeyvalq *)calloc(1, sizeof(struct evkeyvalq));
  evhttp_parse_query(evhttp_decode_uri(uri), params);
  struct evkeyval *header = (evkeyval*)calloc(1, sizeof(struct evkeyval));
  TAILQ_FOREACH(header, params, next) {
    ret[string(header->key)] = string(header->value);
  }
  free(params);
  free(header);
  return ret;
}

string create_simple_response(bool status, const char* reason) {
  ostringstream ss;
  ss << "{'status': " << (status ? "'OK'" : "'NG'");
  ss << ", ";
  ss << "'reason' : '" <<  reason << "'}";
  return ss.str();;
}

void BeatBoard::HTTPAPIServer::connectHandler(struct evhttp_request *req, void *arg) {
  HTTPAPIServer *instance = (HTTPAPIServer*)(arg);
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  struct evbuffer *buf;
  buf = evbuffer_new();

  if (buf == NULL) {
    logger.debug("failed to create response buffer in CONNECT");
    return;
  }

  map<string, string> params = instance->parseParameter(req);
  const string session_id = params[string("sid")];
  const string nick = params[string("nick")];
  const string server = params[string("server")];
  const string port = params[string("port")];
  const string pass = params[string("pass")];
  logger.debug("CONNECT nick:" + nick +
               " server:" + server +
               " port:" + port +
               " pass:" + pass
              );

  IRCConnection *conn = NULL;
  conn = instance->getIRCConnectionBySessionId(session_id);
  ostringstream ss;

  try {
    if (nick.size() == 0) {
      throw BeatBoard::Exception("nick length is 0");
    }

    if (NULL == conn) {
      logger.debug("new connect request arrived. nick:" + nick + ", old sid:" + session_id);
      IRCConnection *newConnection;

      if (instance->isExistNick(nick)) {
        newConnection = instance->getIRCConnectionByNick(nick);
      } else {
        if (pass != string("")) {
          newConnection = new IRCConnection(nick, new string(pass));
        } else {
          newConnection = new IRCConnection(nick);
        }

        newConnection->connectIRCServer(server, port);
      }

      string new_session_id = instance->createSession(newConnection);
      // create new session
      evhttp_add_header(req->output_headers,
                        "Set-Cookie", (string("sid=") + new_session_id + string("; path=/")).c_str());
      ss << create_simple_response(true, "session and connection created");
      logger.debug("created new session for " + nick +
                   ". session_id = " + new_session_id);
      evbuffer_add_printf(buf,  "%s", (char*)(ss.str().c_str()));
    } else {
      map<string, IRCChannel>::iterator it = (conn->received).begin();
      ss << "{'status': 'OK', 'users':{";

      while (it != (conn->received).end()) {
        ss << "'" << it->first << "': [";
        vector<string> users = (it->second).getUsers();
        vector<string>::iterator user = users.begin();

        while (user != users.end()) {
          ss << "'" << *user << "',";
          ++user;
        }

        ss << "],";
        ++it;
      }

      ss << "}}";
      evbuffer_add_printf(buf,  "%s", (char*)(ss.str().c_str()));
    }

    ostringstream csize;
    csize << ss.str().size();
    evhttp_add_header(req->output_headers, "Content-Length", csize.str().c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);

  } catch (BeatBoard::Exception& error) {
    logger.debug("irc connect error" + string(error.message.data()));

    string reason = string("connection create failed. ") + error.message;
    string res = create_simple_response(false, reason.c_str());
    evbuffer_add_printf(buf, "%s", (char*)(res.c_str()));
    ostringstream csize;
    csize << res.size();
    evhttp_add_header(req->output_headers, "Content-Length", csize.str().c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }

  evbuffer_free(buf);
  return;
}

void BeatBoard::HTTPAPIServer::exitHandler(struct evhttp_request *req, void *arg) {
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  struct evbuffer *buf;
  buf = evbuffer_new();

  if (buf == NULL) {
    fprintf(stderr, "failed to create response buffer\n");
    return;
  }

  logger.debug("EXIT");

  try {
    evbuffer_add_printf(buf, "This is EXIT API");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  } catch (BeatBoard::Exception& error) {
    logger.debug("exit error");
    logger.debug(error.message.data());
  }

  evbuffer_free(buf);
  exit(0);
}

void BeatBoard::HTTPAPIServer::joinHandler(struct evhttp_request *req, void *arg) {
  HTTPAPIServer *instance = (HTTPAPIServer*)(arg);
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  struct evbuffer *buf;
  buf = evbuffer_new();

  if (buf == NULL) {
    fprintf(stderr, "failed to create response buffer\n");
    return;
  }

  map<string, string> params = instance->parseParameter(req);
  const string session_id = params[string("sid")];
  const string channel = params[string("channel")];
  logger.debug("JOIN session_id:" + session_id + " channel:" + channel);
  string res;

  try {
    IRCConnection *conn = instance->getIRCConnectionBySessionId(session_id);

    if (conn == NULL) {
      BeatBoard::Exception notfound(string("connection not found"));
      throw notfound;
    }

    conn->JOIN(channel);

    res = create_simple_response(true, "joined");
  } catch (BeatBoard::Exception& error) {
    logger.debug("irc join error");
    logger.debug(error.message.data());
    res = create_simple_response(true, "join failed");
  }

  evbuffer_add_printf(buf, "%s", (char*)(res.c_str()));
  ostringstream csize;
  csize << res.size();
  evhttp_add_header(req->output_headers, "Content-Length", csize.str().c_str());
  evhttp_send_reply(req, HTTP_OK, "OK", buf);
  evbuffer_free(buf);
}

void BeatBoard::HTTPAPIServer::speakHandler(struct evhttp_request *req, void *arg) {
  HTTPAPIServer *instance = (HTTPAPIServer*)(arg);
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();

  struct evbuffer *buf;
  buf = evbuffer_new();

  if (buf == NULL) {
    fprintf(stderr, "failed to create response buffer\n");
    return;
  }

  map<string, string> params = instance->parseParameter(req);
  const string session_id = params[string("sid")];
  const string channel = params[string("channel")];
  const string message = params[string("message")];
  logger.debug("SPEAK session:" + session_id + " channel:" + channel + " message:" + message);


  IRCConnection *conn = instance->getIRCConnectionBySessionId(session_id);

  if (conn == NULL) {
    evbuffer_add_printf(buf, "connection not found");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  } else {
    conn->PRIVMSG(channel, message);
    evbuffer_add_printf(buf, "This is SPEAK API");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
  }

  evbuffer_free(buf);
}

void BeatBoard::HTTPAPIServer::readHandler(struct evhttp_request *req, void *arg) {
  HTTPAPIServer *instance = (HTTPAPIServer*)(arg);
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();

  map<string, string> params = instance->parseParameter(req);
  const string session_id = params[string("sid")];
  logger.debug("READ request accepted. session:" + session_id);

  IRCConnection *conn = NULL;
  struct evbuffer *buf;
  conn = instance->getIRCConnectionBySessionId(session_id);
  buf = evbuffer_new();
  if (buf == NULL) {
    logger.debug("failed to create response buffer in READ");
    return;
  }

  if (conn == NULL) {
    logger.debug("connection not found at read");
    string res = create_simple_response(false, "connection not found");
    evbuffer_add_printf(buf, "%s", (char*)(res.c_str()));
    ostringstream csize;
    csize << res.size();
    evhttp_add_header(req->output_headers, "Content-Length", csize.str().c_str());
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    return;
  }

  HTTPAPIReadNotifier* notifier =   new HTTPAPIReadNotifier(req, timeout);
  // set timeout callback
  struct timeval tv;
  tv.tv_usec = 0;
  tv.tv_sec = timeout;

  evtimer_set(&(notifier->timeout_timer), timeout_timer, req);
  evtimer_add(&(notifier->timeout_timer), &tv);

  instance->setHTTPAPIReadNotifier(session_id,notifier);
  
  evbuffer_free(buf);
}

