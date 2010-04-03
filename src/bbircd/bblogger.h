#ifndef __BB_LOGGER__
#define __BB_LOGGER__

#include <string>
#include <iostream>
#include <time.h>
#include <fstream>

using namespace std;

namespace BeatBoard {
  /*
   * Singleton class
   */
  class BBLogger {
  private:
    BBLogger(void);
    void printLog(const string level, const string message);
    string get_current_time_str();
    std::fstream *lout;
  public:
    static char* logfile;
    void debug(const string message);
    void info(const string message);
    static BBLogger& getInstance(void) {
      static BBLogger singleton;
      return singleton;
    }
  };
}

#endif /* __BB_LOGGER__ */

