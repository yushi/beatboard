#include "tests/bb_sample_rpc_client.h"

int main() {
  BBSampleRpcClient* client = new BBSampleRpcClient();
  client->DoSearch();

  delete client;
  client = NULL;
  return 0;
}
