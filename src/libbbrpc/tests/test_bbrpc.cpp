#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <signal.h>
#include <gtest/gtest.h>

#include "../sample.pb.h"
#include "../bb_rpc_controller.h"
#include "../rpc_server.h"
#include "../bb_rpc_service.h"

#include "bb_sample_rpc.h"
#include "example_service.h"
#include "bb_sample_rpc_client.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ready_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ready_cond = PTHREAD_COND_INITIALIZER;

bool server_start = false;

void* startServer( void* args )
{
  pthread_mutex_lock(&ready_mutex);
  BeatBoard::RpcServer* server = new BeatBoard::RpcServer("127.0.0.1");
  sample::RpcService* service = new ExampleService();
  BeatBoard::BBRpcService* bbservice = new BBSampleRpc( service );
  pthread_cond_signal(&ready_cond);
  pthread_mutex_unlock(&ready_mutex);
  
  server->ExportOnPort(1234, bbservice);
  pthread_mutex_lock(&mutex);
  server_start = true;
  pthread_mutex_unlock(&mutex);
  //pthread_cond_broadcast( &cond );
  pthread_cond_signal( &cond );
  server->Run();

  delete service;
  service = NULL;
  delete server;
  server = NULL;
}

TEST(BBRpcTest, Server) {
  int ret;
  int count;
  pthread_t thread;

  pthread_mutex_lock( &ready_mutex );
  ret = pthread_create(&thread, NULL, startServer, (void*) NULL);
  if (ret != 0)
  {
    perror("pthread_create");
    return;
  }
  pthread_cond_wait( &ready_cond, &ready_mutex );
  pthread_mutex_unlock( &ready_mutex );


  pthread_mutex_lock( &mutex );
  // condition variavle wait and busy loop for server starting
  while (server_start != true)
  {
    if (count >= 5)
    {
      std::cerr << "server start failed" << std::endl;
      break;
    }
    pthread_cond_wait( &cond, &mutex );
    count++;
  }
  pthread_mutex_unlock( &mutex ) ;

  BBSampleRpcClient* client = new BBSampleRpcClient();

  std::string result = client->DoSearch();
  ASSERT_EQ(std::string("not query"), result);

  std::string query = "query";
  result = client->DoSearch2(query);
  ASSERT_EQ(std::string("query"), result);

  // send sigint to event_dispatch() in rpcserver
  pthread_kill(thread, SIGINT);
  //ret = pthread_detach(thread);
  ret = pthread_join(thread, NULL);
  if (ret != 0)
  {
    perror("pthread_join");
    return;
  }

  delete client;
  client = NULL;
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
