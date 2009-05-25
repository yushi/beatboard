.SUFFIXES: .cc .cpp .o .h

CC = g++
CFLAGS = -g -Wall -pipe
OS	= ${shell uname}

LIBLDFLAGS = -lprotobuf -lbbrpc 
LIBS = -L/usr/local/lib -L. -L../libbbrpc/ -L../libbbdrizzle/ -L../libapicommon
INCLUDES = -I../libbbrpc/ -I. -I../libbbdrizzle/ -I../libapicommon

SAMPLECFLAGS = $(CFLAGS)
LDFLAGS = -lprotobuf -lbbrpc
PROTOC = protoc
PROTOFLAGS = --cpp_out=.
PROTOSRC = searchapi.proto
PROTOBUF_SRC = searchapi.pb.cc
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = searchapiserver
SERVER_MAIN = searchapi_service_main.cpp
SERVER_SRCS = $(COMMON_SRCS)\
				searchapi_service.cpp\
				searchapi_service_rpc.cpp\
				searchapi_service_common.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o) 
SERVER_LDFLAGS = $(LDFLAGS) -lbbdrizzle -lapicommon -lmemcached -ljson -ldrizzle

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
EVHTTP_SRCS = ../libbbrpc/sample_rpc_client_evhttp.cpp\
			searchapi_service_sample_client_evhttp.cpp\
			searchapi.pb.cc\
			searchapi_service_common.cpp
EVHTTP_OBJS = $(EVHTTP_SRCS:.cc=.o)

all: $(PROTOBUF_SRC) $(SERVER_TARGET) 

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(INCLUDES) $(SERVER_LDFLAGS) -o $(SERVER_TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

sample: $(PROTOBUF_TARGET) $(CLIENT_TARGET) $(EVHTTP_TARGET)

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
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(CLIENT_TARGET) 
