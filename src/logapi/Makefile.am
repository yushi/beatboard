sbin_PROGRAMS = logapibbevqueue logapiserver logapisampleclient
PROTOFLAGS = --cpp_out=.
AM_LDFLAGS = -L../libbbrpc/.libs -L../libbbevqueue/.libs -L../libapicommon/.libs -L../libbbdrizzle/.libs
AM_CPPFLAGS = -I../libbbrpc/ -I../libbbevqueue -I../libbbdrizzle -I../libapicommon

PROTOBUF_SRC = logapi.pb.cc
PROTOBUF_HEADER = logapi.pb.h
COMMON_SRCS = $(PROTOBUF_SRC)
CLEANFILES = logapi.pb.cc logapi.pb.h
logapibbevqueue_SOURCES = logapi_service_client_bbevqueue_main.cpp\
				logapi_service_client_bbevqueue.cpp\
				logapi_service_common.cpp\
				message.cpp\
				message_map.cpp\
				logapi.pb.cc 
logapibbevqueue_LDADD = -lprotobuf -lbbrpc -lbbevqueue

logapiserver_SOURCES = $(COMMON_SRCS)\
						logapi_service.cpp\
						logapi_service_rpc.cpp\
						logapi_service_common.cpp\
						logapi_service_main.cpp
logapiserver_LDADD = -lprotobuf -lbbrpc -lapicommon -lbbdrizzle

logapisampleclient_SOURCES = $(COMMON_SRCS)\
							 logapi_service_common.cpp\
							 logapi_service_sample_client.cpp
logapisampleclient_LDADD = -lprotobuf -lbbrpc

logapi.pb.h: logapi.proto
	@PROTOC@ $(PROTOFLAGS) $<

logapi_service_client_bbevqueue_main.cpp: logapi.pb.h
