#ifndef QUERY_PARSER_H
#define QUERY_PARSER_H

#include <iostream>
#include "query.h"
#include <stdlib.h>
#include <string.h>
void parse(const char *);

class QueryParser {
private:

public:
  QueryParser();
  ~QueryParser();

//  Query parse( const char query_string[] );
  Query* parse( const std::string& query_string );
};

#endif
