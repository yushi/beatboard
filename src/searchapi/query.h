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
  std::string *ts;
  std::string *id;
  std::string *acontext;
  std::string *bcontext;
  std::string *context;

  Query();
  ~Query();
  
};

#endif
