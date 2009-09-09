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

  ostringstream ss;
  ss << "{";
  map<string, vector<string> >::iterator it = (*messages).begin();
  while( it != (*messages).end() ){
    ss << "\"" << this->escape((*it).first) << "\"";
    ss << ":";
    ss << "[";
    for( unsigned int i = 0; i < (*it).second.size(); i+=2){
      ss << "\"" << this->escape((*it).second[i]) << "\",";
      ss << "\"" << this->escape((*it).second[i+1]) << "\",";
    }
    ss << "],";
    ++it;
  }
  ss << "}";
  
  evhttp_add_header(req->output_headers, "Content-type","application/x-javascript; charset=utf-8");

  ostringstream csize;
  csize << ss.str().size();
  evhttp_add_header(req->output_headers, "Content-Length",csize.str().c_str());

  evbuffer_add( buf, ss.str().c_str(), ss.str().size() );
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
