#include "bb_protobuf_queue_memcached.h"
//TODO hard coding
//TODO type consistency(uint int...)
#include <iostream>

const char* BeatBoard::ProtobufQueueMemcached::index_key = "index";
const int BeatBoard::ProtobufQueueMemcached::uint64_str_len = 21;

BeatBoard::MemcachedValue::MemcachedValue(uint64_t cas, char *value){
  this->cas = cas;
  this->value = (char*)malloc(sizeof(char) * (strlen(value) + 1));
  memcpy(this->value, value, strlen(value) + 1);
}

BeatBoard::MemcachedValue::~MemcachedValue(){
  free(this->value);
}

BeatBoard::ProtobufQueueMemcached::ProtobufQueueMemcached(){
  this->memc = NULL;
  this->servers = NULL;
  this->retry=3;
}


BeatBoard::ProtobufQueueMemcached::~ProtobufQueueMemcached(){
  if(this->servers != NULL){
    memcached_server_list_free(this->servers);
  }
  if(this->memc != NULL){
    memcached_free(this->memc);
  }
}

int BeatBoard::ProtobufQueueMemcached::initQueue(){
  if(this->memc == NULL){
    this->setUp();
  }

  string *serializedIndex = new string();
  QueueIndex index;
  index.set_head(0);
  index.set_tail(0);
  index.SerializeToString(serializedIndex);
  
  int ret = this->_set(this->index_key ,serializedIndex->c_str());
  delete serializedIndex;

  return ret;
}

uint64_t BeatBoard::ProtobufQueueMemcached::addIndex(){
  uint64_t ret = -1;
  MemcachedValue *result;
  char *index_key = (char*)malloc(sizeof(char) * (strlen(this->index_key) + 1));
  memcpy(index_key, this->index_key, strlen(this->index_key) + 1);
  for(int tried = 0; tried < this->retry; tried++){
    // get current index value
    result = this->_get(&index_key);
    if(result == NULL){
      // failed to get index
      continue;
    }
    //TODO overflow check
    QueueIndex index;
    string *serializedIndex = new string();    
    index.ParseFromString(string(result->value));
    index.set_tail(index.tail());
    index.set_head(index.head() + 1);
    index.SerializeToString(serializedIndex);
    // increment index value  

    if(-1 == this->_cas(this->index_key, serializedIndex->c_str(), result->cas)){
      //cas error
      continue;
    }
    ret = index.head() - 1;
    delete(serializedIndex);
    break;
  }

  // clean up
  free(index_key);
  if(result != NULL){
    delete(result);
  }

  return ret;
}

uint64_t BeatBoard::ProtobufQueueMemcached::popIndex(){
  uint64_t ret = -1;
  MemcachedValue *result;
  char *index_key = (char*)malloc(sizeof(char) * (strlen(this->index_key) + 1));
  memcpy(index_key, this->index_key, strlen(this->index_key) + 1);
  for(int tried = 0; tried < this->retry; tried++){
    // get current index value
    result = this->_get(&index_key);
    if(result == NULL){
      // failed to get index
      continue;
    }

    // TODO underflow check
    QueueIndex index;
    QueueIndex newIndex;
    string *serializedIndex = new string();    
    index.ParseFromString(string(result->value));

    newIndex.set_head(index.head());
    newIndex.set_tail(index.tail() + 1);    
    ret = index.tail();
    newIndex.SerializeToString(serializedIndex);
    if(-1 == this->_cas(this->index_key, serializedIndex->c_str(), result->cas)){
      //cas error
      continue;
    }
    delete(serializedIndex);
    
    break;
  }

  // clean up
  free(index_key);
  if(result != NULL){
    delete(result);
  }

  return ret;
}

int BeatBoard::ProtobufQueueMemcached::set(string data){
  if(this->memc == NULL){
    this->setUp();
  }

  for(int tried = 0; tried < this->retry; tried++){
    int next_index;
    if( (next_index = this->addIndex()) == -1){
      continue;
    }
    char next[this->uint64_str_len];
    bzero(next, this->uint64_str_len);
    sprintf(next, "%d", next_index);
    if( this->_set(next, data.c_str()) != 0){
      continue;
    }
    return 0;
  }    
  return -1;
}

string* BeatBoard::ProtobufQueueMemcached::get(){
  if(this->memc == NULL){
    this->setUp();
  }

  char *target = (char *)malloc(sizeof(char) * this->uint64_str_len);
  bzero(target, this->uint64_str_len);
  MemcachedValue *result = NULL;
  for(int tried = 0; tried < this->retry; tried++){
    // get
    int target_int = this->popIndex();
    sprintf(target, "%d", target_int);
    result = this->_get(&target);
    if(result != NULL){
      this->_del(target);
      break;
    }
  }

  string *value = NULL;
  if(result != NULL){
    value = new string(result->value);
    delete(result);
  }
  free(target);
  return value;
}
void BeatBoard::ProtobufQueueMemcached::setUp(){
  memcached_return rc;
  this->memc= memcached_create(NULL);
  memcached_behavior_set (this->memc,
                          MEMCACHED_BEHAVIOR_SUPPORT_CAS,
                          1);
  char servername[]= "localhost";

  servers= memcached_server_list_append(NULL, servername, 11211, &rc);
  if(rc != MEMCACHED_SUCCESS){
    exit(1); //TODO
  }

  rc= memcached_server_push(memc, servers);
  if(rc != MEMCACHED_SUCCESS){
    exit(1); //TODO
  }
}

BeatBoard::MemcachedValue* BeatBoard::ProtobufQueueMemcached::_get(char **key){
  memcached_return rc;
  size_t length = strlen(*key) + 1;
  for(int tried = 0; tried < this->retry; tried++){
    rc = memcached_mget (this->memc,
                         key, &length,
                         1);
    if(rc == MEMCACHED_SUCCESS){
      break;
    }

    if(tried == this->retry){
      //mget failed
      return NULL;
    }
  }
  memcached_result_st *result = NULL;
  for(int tried = 0; tried < this->retry; tried++){
    result =  memcached_fetch_result(this->memc,
                                     NULL,
                                     &rc);

    if(rc == MEMCACHED_SUCCESS){
      break;
    }
    if(tried == this->retry || result == NULL){
      //fetch_result failed
      return NULL;
    }
  }

  MemcachedValue *ret = new MemcachedValue(memcached_result_cas(result),
                                           memcached_result_value(result));

  memcached_result_free(result);
  return ret;
}

int BeatBoard::ProtobufQueueMemcached::_cas(const char *key, const char *val, uint64_t cas){
  memcached_return rc;

  rc = memcached_cas (this->memc,
                      key, strlen(key),
                      val, strlen(val),
                      (time_t)0,
                      (uint32_t)0,
                      cas);

  if(rc == MEMCACHED_SUCCESS){
    return 0;
  }

  return -1;
}

int BeatBoard::ProtobufQueueMemcached::_set(const char *key, const char *val){
  memcached_return rc;

  for(int tried = 0; tried < this->retry; tried++){
    rc = memcached_set (this->memc,
                        key, strlen(key),
                        val, strlen(val),
                        (time_t)0,
                        (uint32_t)0);

    if(rc == MEMCACHED_SUCCESS){
      return 0;
    }
  }

  return -1;
}

int BeatBoard::ProtobufQueueMemcached::_del(const char *key){
  memcached_return rc;

  for(int tried = 0; tried < this->retry; tried++){
    rc = memcached_delete (this->memc,
                           key,
                           strlen(key),
                           (time_t)0);

    if(rc == MEMCACHED_SUCCESS){
      return 0;
    }
  }

  return -1;
}

