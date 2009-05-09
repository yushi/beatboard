require "mkmf"
$CFLAGS += " -Wall -I/usr/bb/local/include"
$LOCAL_LIBS += "-L../ -lbbirc"
create_makefile("bbirc")

