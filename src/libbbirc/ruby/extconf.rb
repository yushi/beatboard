require "mkmf"
$CFLAGS += " -Wall -I/opt/local/include"
#$LIBPATH = "-L.."
$LOCAL_LIBS += "-L../ -lbbirc"
create_makefile("bbirc")

