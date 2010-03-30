BB_HOME=/usr/bb
CC=g++ -g -Wall -Werror
CC_INC_DIR=-I$(BB_HOME)/local/include -I../libbbirc -I../libbbevqueue -I../logapi
CC_LIB_DIR=-L$(BB_HOME)/local/lib -L$(BB_HOME)/lib
CC_LIBS=-levent -lbbirc -lapicommon -lbbevqueue -lbbrpc -ljson
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)
OBJ_FILES=bblogger.o irc_client_operation.o http_api_server.o http_api_notifier.o

all: bbircd

bbircd: $(OBJ_FILES) bbircd.cpp
	$(CC) $(CC_OPTS)  bbircd.cpp $(OBJ_FILES) -o bbircd

.cpp.o:
	$(CC) $(CC_OPTS) -c -I/opt/local/include  $<

test:
	$(CC) $(CC_OPTS) -I/Users/yushi/local/include -L/Users/yushi/local/lib tests/sample.cpp -lgtest -o tests/test
	tests/test

clean:
	rm -rf $(OBJ_FILES) bbircd bbircd.dSYM

install: bbircd
	mkdir -p /usr/bb/sbin
	install -m755 bbircd /usr/bb/sbin
