#include "bb_protobuf_queue_memcached.h"
#include <iostream>
#include <pthread.h>
using namespace std;
int counter = 0;

void* set_func(void *arg){
  int *counter = (int*)arg;
  BeatBoard::ProtobufQueueMemcached queue;
  for(int i = 0; i < 1; i++){
    if(queue.enqueue(string("hogehoge")) != -1){
      *(counter) += 1;
    }
  }
  return 0;
}

int main(){
  int thread_num = 100;
  pthread_t pt[thread_num];
  for(int i = 0; i < thread_num; i++){
    pthread_create(&(pt[i]), NULL, &set_func,(void*)&counter);
  }
  for(int i = 0; i < thread_num; i++){
    pthread_join(pt[i], NULL);
  }

  counter = 0;
  BeatBoard::ProtobufQueueMemcached queue;
  string *value = NULL;
  while( (value = queue.dequeue_nb()) != NULL){
    counter += 1;
    delete value;
    value = NULL;
  }

  cout << "get: " << counter << endl;

  return 0;
}

