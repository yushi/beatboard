#ifndef __BB_LOGGER__
#define __BB_LOGGER__

#include <string>
#include <iostream>
namespace BeatBoard{
  /*
   * Singleton class
   */
  class BBLogger{
  private:
    BBLogger(void);
    void printLog(const std::string level, const std::string message);
  public:
    void debug(const std::string message);
    void info(const std::string message);
    static BBLogger& getInstance(void){
      static BBLogger singleton;
      return singleton;
    }
  };
}


#endif /* __BB_LOGGER__ */

