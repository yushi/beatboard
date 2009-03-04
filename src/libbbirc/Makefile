CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/home/bb/local/include 
CC_LIB_DIR=-L/home/bb/local/lib
CC_LIBS=-levent -lgtest
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)

all: libbbirc.so test_irc_proto
	cd ruby && make -f Makefile.bb

libbbirc.so: irc_connection.cpp
	$(CC) $(CC_OPTS) irc_connection.cpp -shared -o libbbirc.so

irc_proto.c: irc_proto.rl
	ragel -C irc_proto.rl

test_irc_proto: test.cpp irc_proto.c
	$(CC) $(CC_OPTS) irc_proto.c test.cpp -o test_irc_proto

test:
	./test_irc_proto
	
clean:
	rm -rf libbbirc.so test_irc_proto irc_proto.c
	cd ruby && make -f Makefile.bb clean
