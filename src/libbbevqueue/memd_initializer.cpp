#include "bb_protobuf_queue_memcached.h"
#include <iostream>
using namespace std;
int main(){
  BeatBoard::ProtobufQueueMemcached queue;
  queue.initQueue();
  return 0;
}
