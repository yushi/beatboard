require "mkmf"
$CFLAGS += " -Wall -I/bb/local/include"
$LOCAL_LIBS += "-L../ -lbbirc"
create_makefile("bbirc")

