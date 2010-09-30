#include "query.h"

Query::Query() {
  date = NULL;
  channel = NULL;
  order = NULL;
  limit = NULL;
  cache = NULL;
  ts = NULL;
  id = NULL;
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

  if (cache)
  {
    delete cache;
  }

  if (ts)
  {
    delete ts;
  }

  if (id)
  {
    delete id;
  }

  if (!words->empty())
  {
    words->clear();
  }
  delete words;
}
