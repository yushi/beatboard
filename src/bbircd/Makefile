CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/opt/local/include -I../libbbirc
CC_LIB_DIR=-L/opt/local/lib -L../libbbirc
CC_LIBS=-levent -lbbirc
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)
OBJ_FILES=bblogger.o bbircd.o http_api_server.o irc_client_operation.o

all: bbircd

bbircd: $(OBJ_FILES) bbircd.cpp
	$(CC) $(CC_OPTS)  $(OBJ_FILES) -o bbircd

.cpp.o:
	$(CC) $(CC_OPTS) -c -I/opt/local/include  $<

test:
	$(CC) $(CC_OPTS) -I/Users/yushi/local/include -L/Users/yushi/local/lib tests/sample.cpp -lgtest -o tests/test
	tests/test

clean:
	rm $(OBJ_FILES) bbircd
