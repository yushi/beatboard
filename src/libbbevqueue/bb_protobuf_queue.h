#include <string>
using namespace std;

namespace BeatBoard{
  class ProtobufQueue{
  public:
    virtual ~ProtobufQueue(){};
    virtual string* dequeue(){return new string("");};
    virtual int enqueue(string data){return 0;};
  };
}
