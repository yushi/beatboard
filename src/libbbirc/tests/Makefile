OS      = ${shell uname}
ifeq (${OS}, Linux)
ENV_LIBRARY_PATH=LD_LIBRARY_PATH
endif
ifeq (${OS}, Darwin)
ENV_LIBRARY_PATH=DYLD_LIBRARY_PATH
endif

BB_HOME=/usr/bb
CC=g++ -g -Wall -Werror
CC_INC_DIR=-I$(BB_HOME)/local/include -I.. -I../../libbbevqueue -I../../logapi
CC_LIB_DIR=-L$(BB_HOME)/local/lib -L..
CC_LIBS=-lgtest -lbbirc
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)
TEST_LIB_PATH=$(BB_HOME)/local/lib:../:../../libbbevqueue:../../logapi:../../libbbrpc:../../libapicommon

all: test_irc_proto test_irc_connection

test_irc_proto: test_irc_proto.cpp
	LD_LIBRARY_PATH=$(TEST_LIB_PATH) $(CC) $(CC_OPTS) $(CC_LIBS_TEST) $^ -o $@

test_irc_connection: test_irc_connection.cpp
	LD_LIBRARY_PATH=$(TEST_LIB_PATH) $(CC) $(CC_OPTS) $(CC_LIBS_TEST) $^ -o $@

test_irc_channel: test_irc_channel.cpp
	LD_LIBRARY_PATH=$(TEST_LIB_PATH) $(CC) $(CC_OPTS) $(CC_LIBS_TEST) $^ -o $@	

test: test_irc_proto test_irc_connection test_irc_channel
	$(ENV_LIBRARY_PATH)=$(TEST_LIB_PATH) ./test_irc_proto
	$(ENV_LIBRARY_PATH)=$(TEST_LIB_PATH) ./test_irc_connection
	$(ENV_LIBRARY_PATH)=$(TEST_LIB_PATH) ./test_irc_channel

memtest: test_irc_proto
	$(ENV_LIBRARY_PATH)=..:$(BB_HOME)/local/lib valgrind --leak-check=full ./$< 

clean:
	rm -rf test_irc_proto test_irc_connection test_irc_channel test_irc_proto.dSYM test_irc_connection.dSYM test_irc_channel.dSYM
