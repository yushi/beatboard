#include <stdio.h>
#include <libmemcached/memcached.h>
#include <unistd.h>
#include "bb_protobuf_queue.h"
#include "bb_queue_index.pb.h"

namespace BeatBoard{
  class MemcachedValue{
  public:
    MemcachedValue(uint64_t cas, const char *value);
    ~MemcachedValue();
    uint64_t cas;
    char *value;
  };
  
  class ProtobufQueueMemcached:public ProtobufQueue{
  public:
    ProtobufQueueMemcached();
    ~ProtobufQueueMemcached();    
    string* dequeue();
    string* dequeue_nb();
    int enqueue(string data);
    int initQueue();
  private:
    static const char *index_key;
    static const int uint64_str_len;
    int retry;
    void setUp();
    memcached_st *memc;
    memcached_server_st *servers;
    MemcachedValue* _get(char **key);
    uint64_t *addIndex();
    uint64_t *popIndex();
    int _add(const char *key, const char *val);
    int _set(const char *key, const char *val);
    int _cas(const char *key, const char *val, uint64_t cas);
    int _del(const char *key);
  };
  
}
