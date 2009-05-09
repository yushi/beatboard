OS      = ${shell uname}
ifeq (${OS}, Linux)
ENV_LIBRARY_PATH=LD_LIBRARY_PATH
endif
ifeq (${OS}, Darwin)
ENV_LIBRARY_PATH=DYLD_LIBRARY_PATH
endif

CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/bb/local/include 
CC_LIB_DIR=-L/bb/local/lib -L..
CC_LIBS=-lgtest -lbbirc
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)

all: test_irc_proto test_irc_connection

test_irc_proto: test_irc_proto.cpp
	LD_RUN_PATH=/bb/local/lib $(CC) $(CC_OPTS) $(CC_LIBS_TEST) $^ -o $@

test_irc_connection: test_irc_connection.cpp
	LD_RUN_PATH=/bb/local/lib $(CC) $(CC_OPTS) $(CC_LIBS_TEST) $^ -o $@

test: test_irc_proto test_irc_connection
	$(ENV_LIBRARY_PATH)=..:/bb/local/lib ./test_irc_proto
	$(ENV_LIBRARY_PATH)=..:/bb/local/lib ./test_irc_connection

memtest: test_irc_proto
	$(ENV_LIBRARY_PATH)=..:/bb/local/lib valgrind ./$<

clean:
	rm -rf test_irc_proto test_irc_connection test_irc_proto.dSYM test_irc_connection.dSYM
