.SUFFIXES: .cc .cpp .o .h

CC = g++
CFLAGS = -g -Wall -pipe
OS	= ${shell uname}

ifeq (${OS}, Linux)
PROTOC = $(BBHOME)/local/bin/protoc
RAGEL= $(BBHOME)/local/bin/ragel
endif
ifeq (${OS}, Darwin)
PROTOC = protoc
RAGEL= ragel
endif

BBHOME = /usr/bb
BBINCLUDES = -I$(BBHOME)/local/include
BBLIBS = -L$(BBHOME)/local/lib

LIBS = -L/usr/local/lib -L. -L../libbbrpc/ -L../libbbdrizzle/ -L../libapicommon $(BBLIBS)
INCLUDES = -I../libbbrpc/ -I. -I../libbbdrizzle/ -I../libapicommon $(BBINCLUDES)

SAMPLECFLAGS = $(CFLAGS)
LDFLAGS = -lprotobuf -lbbrpc -luuid
PROTOFLAGS = --cpp_out=.
PROTOSRC = searchapi.proto
PROTOBUF_SRC = searchapi.pb.cc
PROTOBUF_HEADER = searchapi.pb.h
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = searchapiserver
SERVER_MAIN = searchapi_service_main.cpp
SERVER_SRCS = $(COMMON_SRCS)\
				searchapi_service.cpp\
				searchapi_service_rpc.cpp\
				searchapi_service_common.cpp\
				query_parser.cpp\
				query.cpp\
				query_logger.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o) 
SERVER_LDFLAGS = $(LDFLAGS) -lbbdrizzle -lapicommon -lmemcached -ljson -ldrizzle
RAGEL_SRC = query_parser.rl
RAGEL_TARGET = $(RAGEL_SRC:.rl=.cpp)	

CLIENT_TARGET = searchapisampleclient
CLIENT_MAIN = searchapi_service_sample_client.cpp
CLIENT_SRCS = $(COMMON_SRCS)\
	      		searchapi_service_common.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

PYTHON_WRRAPER_TARGET = _searchapi_service_sample_client.so
SWIG_SRC = searchapi_service_sample_client.i
SWIG = ~/local/bin/swig
SWIG_OPT =-fPIC -c

SWIG_LIB_OPT = -G -dynamiclib
SWIG_LDFLAGS = -lprotobuf -lbbrpc -lpython

SWIG_SRCS = searchapi_service_sample_client.cpp\
			searchapi_service_sample_client_wrap.cxx\
			searchapi.pb.cc
SWIG_OBJS = $(SWIG_SRCS:.cpp=.o)

EVHTTP_TARGET = evhttp_search_client
EVHTTP_SRCS = ../libbbrpc/bb_rpc_client_evhttp.cpp\
			searchapi.pb.cc\
			searchapi_service_common.cpp\
			searchapi_service_client_evhttp.cpp\
			searchapi_service_client_evhttp_main.cpp
#			searchapi_service_sample_client_evhttp.cpp
EVHTTP_OBJS = $(EVHTTP_SRCS:.cc=.o)

all: $(PROTOBUF_SRC) $(SERVER_TARGET) $(EVHTTP_TARGET)

$(SERVER_TARGET): $(RAGEL_TARGET) $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(INCLUDES) $(SERVER_LDFLAGS) -o $(SERVER_TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

$(RAGEL_TARGET):
	$(RAGEL) -C $(RAGEL_SRC) -o $(RAGEL_SRC:.rl=.cpp)

sample: $(PROTOBUF_TARGET) $(CLIENT_TARGET)

$(EVHTTP_TARGET):
	$(CC) $(CFLAGS) $(EVHTTP_SRCS) -o $(EVHTTP_TARGET) $(LIBS) $(INCLUDES) $(LDFLAGS) -levent

$(PROTOBUF_SRC):
	$(PROTOC) $(PROTOFLAGS) $(PROTOSRC)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(SAMPLECFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $(CLIENT_TARGET)

$(PYTHON_WRRAPER_TARGET): $(PYTHON_WRRAPER_OBJS)
	$(SWIG) -c++ -python $(SWIG_SRC)
	$(CC) $(SWIG_OPT)  $(SWIG_SRCS) -I../libbbrpc/ -I/opt/local/include/python2.5/
	$(CC) $(SWIG_LIB_OPT) $(SWIG_OBJS) -o $(PYTHON_WRRAPER_TARGET)  -L. -L../libbbrpc/. -L/opt/local/lib/python2.5  $(SWIG_LDFLAGS)

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(CLIENT_TARGET) $(EVHTTP_TARGET) $(PROTOBUF_HEADER)
