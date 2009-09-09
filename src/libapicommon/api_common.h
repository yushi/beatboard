#ifndef API_COMMON_H
#define API_COMMON_H

#include <iostream>
#include <string>
#include <vector>

namespace BeatBoard {
  class ApiCommon {

  public:
    static void replaceEscapeChar( std::string& data,
								   std::string& escapechar,
								   std::string& replacechar );
    static std::string escape( const std::string& data );
  };
}

#endif
