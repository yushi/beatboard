#include "bb_protobuf_queue_memcached.h"
#include <iostream>
//#include <stdio.h>
using namespace std;
int iteration(int count){
  BeatBoard::ProtobufQueueMemcached queue;

  for(int i = 0; i < count; i++){
    queue.enqueue(string("koko ni protobuf no data ireruyo-"));
  }
    
  string *value;
  while((value = queue.dequeue_nb()) != NULL){
    cout << *value << endl;
    delete value;
  }
  return 0;
}

int recv(){
  BeatBoard::ProtobufQueueMemcached queue;

  string *value;
  while((value = queue.dequeue()) != NULL){
    cout << *value << endl;
    delete value;
  }
  return 0;
}

int main(int argc, char **argv){
  const int MODE_ITERATION = 0;
  const int MODE_RECV = 1;
  
  int opt;
  int mode = MODE_ITERATION;
  int numiteration = 1;
  
  while ((opt = getopt(argc, argv, "rin:")) != -1) {
    switch (opt) {
    case 'i':
      mode = MODE_ITERATION;
      break;

    case 'r':
      mode = MODE_RECV;
      break;

    case 'n':
      numiteration = atoi(optarg);
      break;

    default: /* '?' */
      fprintf(stderr, "Usage: %s -i [-n num_iteration]\n",
              argv[0]);
      fprintf(stderr, "Usage: %s -r\n",
              argv[0]);
      exit(-1);
    }
  }

  if(mode == MODE_ITERATION){
    return iteration(numiteration);
  }else if(mode == MODE_RECV){
    return recv();
  }
}
