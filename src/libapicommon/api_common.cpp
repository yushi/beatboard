#include "api_common.h"

static const std::string comma = ",";
static const std::string doublequotation = "\"";
static const std::string singlequotation = "'";
static const std::string backslash = "\\";

void
BeatBoard::ApiCommon::replaceEscapeChar( std::string& data, std::string& escapechar)
{
  int position = data.find(escapechar);
  while (position != -1)
  {
    data.replace( position, 1, backslash + escapechar, 0, escapechar.size() + backslash.size());
    position = data.find( ".", position + 1 );
    std::cout << "data: " << data << std::endl;
  }
}

std::string
BeatBoard::ApiCommon::escape( const std::string& data )
{
  std::vector<std::string> escape_char;
  escape_char.push_back(backslash);
  escape_char.push_back(comma);
  escape_char.push_back(doublequotation);
  escape_char.push_back(singlequotation);

  std::string escaped_data = data;
  std::vector<std::string>::iterator it = escape_char.begin();
  for( ; it != escape_char.end(); ++it )
  {
    std::cout << *it << std::endl;
    replaceEscapeChar( escaped_data, *it );
  }
  return escaped_data;
}

