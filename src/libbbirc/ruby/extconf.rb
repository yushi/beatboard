require "mkmf"
$CFLAGS += " -Wall -I/home/bb/local/include"
#$LIBPATH = "-L.."
$LOCAL_LIBS += "-L../ -lbbirc"
create_makefile("bbirc")

