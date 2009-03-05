CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/home/bb/local/include 
CC_LIB_DIR=-L/home/bb/local/lib
CC_LIBS=-levent
CC_LIBS_TEST=-lgtest
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)
OBJS=irc_proto.o irc_connection.o
LIBBBIRC=libbbirc.so

.cpp.o:
	$(CC) $(CC_OPTS) -c $<

all: $(LIBBBIRC) $(OBJS)
	make -C ruby -f Makefile.bb
	make -C tests

libbbirc.so: $(OBJS)
	$(CC) $(CC_OPTS) $^ -shared -o $@

irc_proto.cpp: irc_proto.rl
	ragel -C $< -o $@

test: all
	make -C tests test

memtest: all
	make -C tests memtest

clean:
	rm -rf $(LIBBBIRC) $(OBJS) irc_proto.cpp
	make -C ruby -f Makefile.bb clean
	make -C tests clean
