lib_LTLIBRARIES = libbbirc.la
libbbirc_la_SOURCES = irc_connection.cpp irc_channel.cpp irc_proto.cpp \
                      irc_connection.h irc_channel.h irc_proto.h bb_exception.h
libbbirc_la_LIBADD = -levent -lbbevqueue -lapicommon -lprotobuf ../logapi/logapi.pb.o
CLEANFILES = 
AM_LDFLAGS = -L../libbbevqueue/.libs -L../libapicommon/.libs -fPIC
AM_CPPFLAGS = -I../libbbevqueue/ -I../logapi
EXTRA_DIST = irc_proto.rl

irc_connection.cpp: irc_proto.cpp
irc_proto.cpp: irc_proto.rl
	@RAGEL@ -C $< -o $@
