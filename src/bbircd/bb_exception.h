#ifndef __BB_EXCEPTION__
#define __BB_EXCEPTION__
#include <string>
namespace BeatBoard{
  class Exception{
  public:
    const std::string message;
  Exception(const std::string& error): message(error){};
  };

}

#endif /* __BB_EXCEPTION__ */
