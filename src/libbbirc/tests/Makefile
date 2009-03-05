CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/home/bb/local/include 
CC_LIB_DIR=-L/home/bb/local/lib -L..
CC_LIBS=-lgtest -lbbirc
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)

all: test_irc_proto

test_irc_proto: test.cpp
	$(CC) $(CC_OPTS) $(CC_LIBS_TEST) $^ -o $@

test: test_irc_proto
	LD_LIBRARY_PATH=.. ./$<

memtest: test_irc_proto
	LD_LIBRARY_PATH=.. valgrind ./$<

clean:
	rm -rf test_irc_proto
