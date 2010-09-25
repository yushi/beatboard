#include "query_parser.h"

%%{
  machine bbquery_parser;

  action begin_query { begin = fpc; }
  action end_query {
    puts("parse query");
    query->words->push_back(substr(begin, fpc));
  }

  action begin_date { begin = fpc; }
  action end_date {
    puts("parse date");
    query->date = substr(begin, fpc);
  }

  action begin_channel { begin = fpc; }
  action end_channel {
    puts("parse channel");
    query->channel = substr(begin, fpc);
  }

  action begin_order { begin = fpc; }
  action end_order {
    puts("parse order by");
    query->order = substr(begin, fpc);
  }

  action begin_limit { begin = fpc; }
  action end_limit {
    puts("parse limit");
    query->limit = substr(begin, fpc);
  }

  action begin_cache { begin = fpc; }
  action end_cache {
    puts("parse cache");
    query->cache = substr(begin, fpc);
  }

  action begin_ts { begin = fpc; }
  action end_ts {
    puts("parse ts");
    query->ts = substr(begin, fpc);
  }

  action error { 
    puts("parse error");
    parse_result = false;
  }

  words_chstring = (0x01..0x07 | 0x08..0x09 | 0x0b..0x0c | 0x0e..0x1f | 0x21..0x2b | 0x2d..0x39 | 0x3b..0x7f | -128..-1);
  sp = space;
  colon = ":";
  sharp = "#";
  andch = "&";
  irc_ch_prefix = ( sharp | andch );
  cache = "cache" colon ( "on" | "off" ) >begin_cache %end_cache;
  order = "order" colon ( "asc" | "desc" ) >begin_order %end_order;
  limit = "limit" colon (digit){1,4} >begin_limit %end_limit;
  channel = "channel" colon (irc_ch_prefix words_chstring{1,}) >begin_channel %end_channel;
  date = "date" colon (digit){8} >begin_date %end_date;
  ts = "ts" colon (digit){10} >begin_ts %end_ts;
  options = date | channel | order | limit | cache | ts;
  words = (words_chstring){1,} >begin_query %end_query;  
  label = (words | options);
  query = sp? label sp{1,} (label sp)*;
  main := query @!error;

}%%

%% write data;

std::string* substr(const char* start, const char* end)
{
  char *buf = (char *)calloc((end - start + 1), sizeof(char));
  std::string *str;
  memcpy(buf, start, end - start);
  str = new std::string(buf);
  free(buf);
  return str;
}

QueryParser::QueryParser() {
}

QueryParser::~QueryParser() {
}

Query*
QueryParser::parse( const std::string& query_string ) {
  Query *query = new Query();

  const char* p = query_string.c_str();
  const char* pe = (const char*)(query_string.length() + query_string.length());
  const char* eof = pe;
  int cs;
  const char* begin = NULL;
  bool parse_result = true;

  std::cout << "***" << query_string << std::endl;

  %% write init;
  %% write exec;

//   if (query->date)
//     std::cerr << *(query->date) << std::endl;
//   if (query->channel)
//     std::cerr << *(query->channel) << std::endl;
//   if (!query->words->empty())
//   {
//     std::vector<std::string*>::iterator it = query->words->begin();
//     while (it != query->words->end() )
//     {
//       std::cerr << *(*it) << std::endl;
//       ++it;
//     }
//   }

  if (parse_result)
  {
    return query;
  }
  else
  {
    return NULL;
  }
}
