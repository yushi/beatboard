CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/home/bb/local/include -I../libbbirc -I../libbbevqueue
CC_LIB_DIR=-L/home/bb/local/lib -L../libbbirc -L../libbbevqueue
CC_LIBS=-levent -lbbirc -lbbevqueue -lprotobuf -lmemcached
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)
OBJ_FILES=bblogger.o irc_client_operation.o http_api_server.o

all: bbircd

bbircd: $(OBJ_FILES) bbircd.cpp
	$(CC) $(CC_OPTS)  bbircd.cpp $(OBJ_FILES) -o bbircd

.cpp.o:
	$(CC) $(CC_OPTS) -c -I/opt/local/include  $<

test:
	$(CC) $(CC_OPTS) -I/Users/yushi/local/include -L/Users/yushi/local/lib tests/sample.cpp -lgtest -o tests/test
	tests/test

clean:
	rm $(OBJ_FILES) bbircd
