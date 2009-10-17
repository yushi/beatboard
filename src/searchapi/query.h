#ifndef QUERY_H
#define QUERY_H

#include <iostream>
#include <string>
#include <vector>

#include <stdio.h>

class Query {
private:
public:
  std::string *date;
  std::string *channel;
  std::vector<std::string*> *words;
  std::string *order;
  std::string *limit;
  std::string *cache;

  Query();
  ~Query();
  
};

#endif
