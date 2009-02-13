CC=g++ -g -Wall -Werror
CC_INC_DIR=-I/opt/local/include 
CC_LIB_DIR=-L/opt/local/lib
CC_LIBS=-levent
CC_OPTS=$(CC_LIB_DIR) $(CC_INC_DIR) $(CC_LIBS)

all: libbbirc

libbbirc: $(OBJ_FILES) irc_connection.cpp
	$(CC) $(CC_OPTS) irc_connection.cpp -dynamiclib -o libbbirc.dylib

clean:
	rm -rf libbbirc.dylib libbbirc.dylib.dSYM
