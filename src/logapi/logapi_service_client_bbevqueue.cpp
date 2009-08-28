#include "logapi_service_client_bbevqueue.h"

BeatBoard::LogApiServiceClientBbevqueue::LogApiServiceClientBbevqueue(
  const std::string& rpcserver_host,
  const int rpcserver_port
  )
{
  this->rpcserver_host = rpcserver_host;
  this->rpcserver_port = rpcserver_port;
  
  this->message_expiration = 2;
  messagemap = new MessageMap(5); // default message expiration check time is 5sec
}

BeatBoard::LogApiServiceClientBbevqueue::~LogApiServiceClientBbevqueue()
{
  delete service;
  delete channel;
  delete controller;
  delete messagemap;
}

void
BeatBoard::LogApiServiceClientBbevqueue::callback()
{
}

bool
BeatBoard::LogApiServiceClientBbevqueue::checkMessageDuplication( const std::string& key )
{
  std::cerr << __func__ << std::endl;
  bool result = messagemap->checkMessageDuplication(key);
  return result;
}

std::string
BeatBoard::LogApiServiceClientBbevqueue::removeIPaddressFromIdentifier()
{
  std::string delimiter = "!";
  std::string identifier = std::string(request.identifier());
  int position =  identifier.find(delimiter);
  std::string identifier_substr = identifier.substr(0, position);
  return identifier_substr;
}

void
BeatBoard::LogApiServiceClientBbevqueue::insert()
{
  channel = new BeatBoard::BBRpcChannel(rpcserver_host, rpcserver_port);
  controller = new BeatBoard::BBRpcController();
  service = new logapi::RpcService::Stub(channel);

  std::cerr << __func__ << std::endl;
//  std::cerr << "channel: " << request.channel() << std::endl;
//  std::cerr << "time: " << request.time() << std::endl;
//  std::cerr << "identifier: " << request.identifier() << std::endl;
//  std::cerr << "message: " << request.message() << std::endl;
  
  google::protobuf::Closure* cb = google::protobuf::NewCallback(&LogApiServiceClientBbevqueue::callback);
  service->RpcFunc(controller, &request, &response, cb);
}

bool
BeatBoard::LogApiServiceClientBbevqueue::dequeueLogData()
{
  BeatBoard::ProtobufQueueMemcached queue;
  std::string *value = NULL;

  if ((value = queue.dequeue_nb()) != NULL)
  {
    //std::cerr << *value << std::endl;
    request.ParseFromString(*value);

    std::string identifier = removeIPaddressFromIdentifier();
    std::string key = request.channel() + request.time() + identifier + request.message();
    std::cerr << "key: " << key << std::endl;
    
    if (checkMessageDuplication(key))
    {
      return true;
    }

    messagemap->setMessage(key, message_expiration);
    insert();

    std::cerr << "result: " << response.result() << std::endl;
    unsigned int result = response.result();
    if (result == LOGAPI_RESULT_OK)
    {
      std::cerr << "insert success" << std::endl;
    }
    else if (result == LOGAPI_RESULT_ERROR)  // 蜀榊ｺｦ謚募�
    {
      std::cerr << "insert failed" << std::endl;
      insert();
    }
    else
    {
      std::cerr << "unknow response" << std::endl;
    }

    delete value;
    value = NULL;
    return true;
  }
  else
  {
    return false;
  }
}

void
BeatBoard::LogApiServiceClientBbevqueue::start()
{
  bool ret = false;
  while(1) {
    ret = dequeueLogData();
    if (!ret)
    {
      //std::cerr << "bbevqueue is empty" << std::endl;
      usleep(20);
    }
  }
}
