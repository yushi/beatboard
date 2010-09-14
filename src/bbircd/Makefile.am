sbin_PROGRAMS = bbircd
bbircd_SOURCES = bblogger.cpp http_api_server.cpp http_api_notifier.cpp session_manager.cpp bbircd.cpp
bbircd_LDADD = -lprotobuf -levent -ljson -lbbirc
AM_CPPFLAGS = -I../libbbirc -I../libbbevqueue -I../logapi
AM_LDFLAGS = -L../libbbirc
