all:
	g++ privmsg.cpp -I ../libbbevqueue/ -I ~/local/include/ -L ../libbbevqueue -L ~/local/lib -lcgicc -o privmsg.cgi -lbbevqueue -lprotobuf -lmemcached
	g++ connect.cpp -I ../libbbevqueue/ -I ~/local/include/ -L ../libbbevqueue -L ~/local/lib -lcgicc -o connect.cgi -lbbevqueue -lprotobuf -lmemcached
	g++ join.cpp -I ../libbbevqueue/ -I ~/local/include/ -L ../libbbevqueue -L ~/local/lib -lcgicc -o join.cgi -lbbevqueue -lprotobuf -lmemcached

