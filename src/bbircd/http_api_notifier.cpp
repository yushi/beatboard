#include <event.h>
#include <evhttp.h>
#include "bblogger.h"
#include "http_api_notifier.h"

BeatBoard::HTTPAPIReadNotifier::HTTPAPIReadNotifier(struct evhttp_request *req){
  this->req = req;
  this->init_time = time(NULL);
}

BeatBoard::HTTPAPIReadNotifier::~HTTPAPIReadNotifier(){
}

bool BeatBoard::HTTPAPIReadNotifier::notify(map<string, vector<string> >* messages){
  BeatBoard::BBLogger logger = BeatBoard::BBLogger::getInstance();
  logger.debug("NOTIFY");
  evhttp_request* req = this->req;
  this->req = NULL;

  struct evbuffer *buf;
  buf = evbuffer_new();
  if ( buf == NULL ) {
    fprintf( stderr, "failed to create response buffer\n" );
    return false;
  }

  string resp = string("{");
  map<string, vector<string> >::iterator it = (*messages).begin();
  while( it != (*messages).end() ){
    string key = "\"" + this->escape((*it).first) + "\"";

    
    string val = "[";
    for( unsigned int i = 0; i < (*it).second.size(); i+=2){
      val += "\"" + this->escape((*it).second[i]) + "\",";
      val += "\"" + this->escape((*it).second[i+1]) + "\",";
    }
    val += "]";
    resp += key + ":" + val + ",";
    ++it;
  }
  resp += "}";

  evhttp_add_header(req->output_headers, "Content-type","application/x-javascript; charset=utf-8");

  ostringstream csize;
  csize << resp.size();
  evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());

  evbuffer_add( buf, resp.c_str(), resp.size() );
  evhttp_send_reply( req, HTTP_OK, "OK", buf );
  logger.debug("NOTIFY END");
  evbuffer_free(buf);
  return true;
}

string BeatBoard::HTTPAPIReadNotifier::escape(string str){
  stringstream ret;
  for(unsigned int i = 0; i < str.size(); i++){
    if(str[i] == '\\'){
      ret << "\\\\";
    }else if(str[i] == '"'){
      ret << "\\\"";
    }else{
      ret << str[i];
    }
  }
  return ret.str();
}
