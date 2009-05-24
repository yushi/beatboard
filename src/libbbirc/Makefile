OS      = ${shell uname}
ifeq (${OS}, Linux)
LIBBBIRC=libbbirc.so
CLIBFLAGS = -shared
endif
ifeq (${OS}, Darwin)
LIBBBIRC=libbbirc.dylib
CLIBFLAGS = -dynamiclib
endif

BB_HOME=/usr/bb
CC=g++ -g -Wall -Werror -fprofile-arcs -ftest-coverage -fPIC
CC_INC_DIR=-I$(BB_HOME)/local/include
CC_LIB_DIR=-L$(BB_HOME)/local/lib
CC_LIBS=-levent
CC_LIBS_TEST=-lgtest
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)
OBJS=irc_proto.o irc_connection.o
COVER_INFO=libbbirc.info
COVER_DIR=cov
RAGEL=$(BB_HOME)/local/bin/ragel
.cpp.o:
	$(CC) $(CC_OPTS) -c $<

all: $(LIBBBIRC) $(OBJS) irc_client
	make -C ruby -f Makefile.bb
	make -C tests

$(LIBBBIRC): $(OBJS)
	$(CC) $(CC_OPTS) $^ $(CLIBFLAGS) -o $@

irc_proto.cpp: irc_proto.rl
	$(RAGEL) -C $< -o $@

test: all
	make -C tests test
	lcov -c -d . -o $(COVER_INFO)
	genhtml -o $(COVER_DIR) $(COVER_INFO)

memtest: all
	make -C tests memtest

irc_client: irc_client.cpp $(LIBBBIRC)
	$(CC) $(CC_OPTS) $< -o irc_client -lpthread -lbbirc -L.

run_irc_client: irc_client
	LD_LIBRARY_PATH=. valgrind --leak-check=full --show-reachable=yes ./irc_client

clean:
	rm -rf $(LIBBBIRC) $(OBJS) irc_proto.cpp irc_client irc_client.dSYM
	rm -rf *.gcno *.gcda $(COVER_INFO) $(COVER_DIR)
	make -C ruby -f Makefile.bb clean
	make -C tests clean
