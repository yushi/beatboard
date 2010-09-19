sbin_PROGRAMS = searchapiserver evhttp_search_client
PROTOFLAGS = --cpp_out=.
AM_LDFLAGS = -L../libbbrpc/.libs -L../libapicommon/.libs -L../libbbdrizzle/.libs
AM_CPPFLAGS = -I../libbbrpc/ -I../libbbdrizzle -I../libapicommon

PROTOBUF_SRC = searchapi.pb.cc
PROTOBUF_HEADER = searchapi.pb.h
COMMON_SRCS = $(PROTOBUF_SRC)
CLEANFILES = logapi.pb.cc logapi.pb.h

searchapiserver_SOURCES = $(COMMON_SRCS)\
						searchapi_service.cpp\
						searchapi_service.h\
						searchapi_service_rpc.cpp\
						searchapi_service_rpc.h\
						searchapi_service_common.cpp\
						searchapi_service_common.h\
						query_parser.cpp\
						query_parser.h\
						query.cpp\
						query.h\
						query_logger.cpp\
						query_logger.h\
						searchapi_service_main.cpp
searchapiserver_LDADD = -lprotobuf -lbbrpc -lbbdrizzle -lapicommon -lmemcached -ljson -ldrizzle -luuid

evhttp_search_client_SOURCES = $(COMMON_SRCS)\
						../libbbrpc/bb_rpc_client_evhttp.cpp\
						../libbbrpc/bb_rpc_client_evhttp.h\
						searchapi_service_common.cpp\
						searchapi_service_common.h\
						searchapi_service_client_evhttp.cpp\
						searchapi_service_client_evhttp.h\
						searchapi_service_client_evhttp_main.cpp
evhttp_search_client_LDADD = -lprotobuf -lbbrpc -lapicommon

searchapi.pb.h: searchapi.proto
	@PROTOC@ $(PROTOFLAGS) $<

query_parser.cpp: query_parser.rl
	@RAGEL@ -C $< -o $@