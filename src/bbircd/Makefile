CC=g++ -g -Wall -Werror
CC_INC_DIR=/opt/local/include
CC_LIB_DIR=/opt/local/lib
CC_LIBS=-llog4cpp -levent
CC_OPTS=-L$(CC_LIB_DIR) -I$(CC_INC_DIR) $(CC_LIBS)
OBJ_FILES=irc_connection.o bblogger.o bbircd.o http_api_server.o irc_client_operation.o

all: bbircd

bbircd: $(OBJ_FILES) bbircd.cpp
	$(CC) $(CC_OPTS)  $(OBJ_FILES) -o bbircd

.cpp.o:
	$(CC) -c -I/opt/local/include  $<

test:
	$(CC) $(CC_OPTS) -I/Users/yushi/local/include -L/Users/yushi/local/lib tests/sample.cpp -lgtest -o tests/test
	tests/test

clean:
	rm $(OBJ_FILES) bbircd
