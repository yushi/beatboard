#include "query.h"

Query::Query() {
  date = NULL;
  channel = NULL;
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

  if (!words->empty())
  {
    words->clear();
  }
  delete words;
}
