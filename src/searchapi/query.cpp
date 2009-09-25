#include "query.h"

Query::Query() {
  date = NULL;
  channel = NULL;
  order = NULL;
  limit = NULL;
  words = new std::vector<std::string*>;
}

Query::~Query() {
  if (date) 
  {
    delete date;
  }

  if (channel)
  {
    delete channel;
  }

  if (order)
  {
    delete order;
  }

  if (limit)
  {
    delete limit;
  }

  if (!words->empty())
  {
    words->clear();
  }
  delete words;
}
