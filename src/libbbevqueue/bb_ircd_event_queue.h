#include "bb_protobuf_queue_memcached.h"
#include "bb_ircd_event.pb.h"

namespace BeatBoard{
  namespace BBIRCD{
    const int TYPE_CONNECT = 1;
    const int TYPE_PRIVMSG = 2;
    const int TYPE_JOIN = 3;
    class EventQueue{
    public:
      EventQueue();
      ~EventQueue();
      Event dequeue();
      int enqueue(Event);
    private:
      BeatBoard::ProtobufQueueMemcached queue; 
    };
  }
}
