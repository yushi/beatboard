#include "bb_protobuf_queue_memcached.h"
#include <iostream>
//#include <stdio.h>
using namespace std;
int main(){
  BeatBoard::ProtobufQueueMemcached queue;

  for(int i = 0; i < 1; i++){
    queue.enqueue(string("koko ni protobuf no data ireruyo-"));
  }

  string *value;
  while((value = queue.dequeue_nb()) != NULL){
      cout << *value << endl;
      delete value;
  }
  return 0;
}
