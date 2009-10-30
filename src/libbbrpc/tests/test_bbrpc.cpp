#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <fstream>
#include <string>
#include <pthread.h>
#include <gtest/gtest.h>

#include "../sample.pb.h"
#include "../bb_rpc_controller.h"
#include "../rpc_server.h"
#include "../bb_rpc_service.h"

#include "bb_sample_rpc.h"
#include "example_service.h"
#include "bb_sample_rpc_client.h"

bool server_start = false;

void* startServer( void* args )
{
  BeatBoard::RpcServer* server = new BeatBoard::RpcServer("127.0.0.1");
  sample::RpcService* service = new ExampleService;
  BeatBoard::BBRpcService* bbservice = new BBSampleRpc( service );
  
  server->ExportOnPort(1234, bbservice);
  server_start = true;
  server->Run();

  delete service;
  service = NULL;
}

TEST(BBRpcTest, Server) {
  int ret;
  int count;
  pthread_t thread;

  ret = pthread_create(&thread, NULL, startServer, (void*) NULL);
  if (ret != 0)
  {
    perror("pthread_create");
    return;
  }

  // busy loop for server starting
  while (server_start != true)
  {
    if (count >= 5)
    {
      std::cerr << "server start failed" << std::endl;
    }
    sleep(1);
    count++;
  }

  BBSampleRpcClient* client = new BBSampleRpcClient();

  std::string result = client->DoSearch();
  ASSERT_EQ(std::string("not query"), result);

  std::string query = "query";
  result = client->DoSearch2(query);
  ASSERT_EQ(std::string("query"), result);

  ret = pthread_detach(thread);
  if (ret != 0)
  {
    perror("pthread_detach");
    return;
  }
  sleep( 2 );
  ret = pthread_cancel(thread);
  if (ret != 0)
  {
    perror("pthread_cancel");
    return;
  }

  delete client;
  client = NULL;
}


int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
