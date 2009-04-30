#ifndef API_COMMON_H
#define API_COMMON_H

#include <iostream>
#include <string>
#include <vector>

namespace BeatBoard {
  class ApiCommon {
  private:
    static void replaceEscapeChar( std::string& data, std::string& escapechar);

  public:
    static std::string escape( const std::string& data );
  };
}

#endif
