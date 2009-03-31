#include <string>
using namespace std;

namespace BeatBoard{
  class ProtobufQueue{
  public:
    virtual ~ProtobufQueue(){};
    virtual string* get(){return new string("");};
    virtual int set(string data){return 0;};
  };
}
