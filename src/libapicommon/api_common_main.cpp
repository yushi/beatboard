#include <iostream>

#include "api_common.h"

int main()
{
  std::string query = "h,o\"g'e";
  std::cout << "query: " << BeatBoard::ApiCommon::escape(query) << std::endl;
  return 0;
}
