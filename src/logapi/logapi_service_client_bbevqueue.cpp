#include "logapi_service_client_bbevqueue.h"

BeatBoard::LogApiServiceClientBbevqueue::LogApiServiceClientBbevqueue(
  const std::string& rpcserver_host,
  const int rpcserver_port
  )
{
  this->rpcserver_host = rpcserver_host;
  this->rpcserver_port = rpcserver_port;
}

BeatBoard::LogApiServiceClientBbevqueue::~LogApiServiceClientBbevqueue()
{
  delete service;
  delete channel;
  delete controller;
}

void
BeatBoard::LogApiServiceClientBbevqueue::callback()
{
}

bool
BeatBoard::LogApiServiceClientBbevqueue::dequeueLogData()
{
  BeatBoard::ProtobufQueueMemcached queue;
  std::string *value = NULL;

  if ((value = queue.dequeue_nb()) != NULL)
  {
    channel = new BeatBoard::BBRpcChannel(rpcserver_host, rpcserver_port);
    controller = new BeatBoard::BBRpcController();

    service = new logapi::RpcService::Stub(channel);

    std::cerr << *value << std::endl;
    request.ParseFromString(*value);

    std::cerr << "channel: " << request.channel() << std::endl;
    std::cerr << "time: " << request.time() << std::endl;
    std::cerr << "identifier: " << request.identifier() << std::endl;
    std::cerr << "message: " << request.message() << std::endl;

    google::protobuf::Closure* cb = google::protobuf::NewCallback(&LogApiServiceClientBbevqueue::callback);
    service->RpcFunc(controller, &request, &response, cb);

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
      std::cerr << "bbevqueue is empty" << std::endl;
      usleep(20);
    }
  }
}
