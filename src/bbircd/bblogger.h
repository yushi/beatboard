#ifndef __BB_LOGGER__
#define __BB_LOGGER__

#include "log4cpp_def.h"
#include <string>
namespace BeatBoard{
  /*
   *Singleton class
   * this class is log4cpp wrapper
   */
  class BBLogger{
  private:
    BBLogger(void);
    log4cpp::Category *logger;
  public:
    void debug(const std::string message);
    static BBLogger& getInstance(void){
      static BBLogger singleton;
      return singleton;
    }
  };
}


#endif /* __BB_LOGGER__ */

