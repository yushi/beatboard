.SUFFIXES: .cc .cpp .o .h

CC = g++
CFLAGS = -g -Wall -pipe
OS	= ${shell uname}

ifeq ($(BB), 1)
BBHOME = /usr/bb
BBINCLUDES = -I$(BBHOME)/local/include
BBLIBS = -L$(BBHOME)/local/lib
else
BBINCLUDES=
BBLIBS=
endif

LIBLDFLAGS = -lprotobuf -lbbrpc -luuid
LIBS = -L/usr/local/lib -L. -L../libbbrpc/ -L../libbbdrizzle/ -L../libapicommon $(BBLIBS)
INCLUDES = -I../libbbrpc/ -I. -I../libbbdrizzle/ -I../libapicommon $(BBINCLUDES)

SAMPLECFLAGS = $(COMMON_CFLAGS)
LDFLAGS = -lprotobuf -lbbrpc -luuid
PROTOC = protoc
PROTOFLAGS = --cpp_out=.
PROTOSRC = authapi.proto
PROTOBUF_SRC = authapi.pb.cc
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = authapiserver
SERVER_MAIN = authapi_service_main.cpp
SERVER_SRCS = $(COMMON_SRCS)\
				authapi_service.cpp\
				authapi_service_rpc.cpp\
				authapi_service_common.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o) 
SERVER_LDFLAGS = $(LDFLAGS) -lbbdrizzle -lapicommon -ldrizzle

CLIENT_TARGET = authapisampleclient
CLIENT_MAIN = authapi_service_sample_client.cpp
CLIENT_SRCS = $(COMMON_SRCS)\
	      		authapi_service_common.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

EVHTTP_LDFLAGS = $(LDFLAGS) -levent -lapicommon
EVHTTP_TARGET = evhttp_auth_client
EVHTTP_SRCS = ../libbbrpc/bb_rpc_client_evhttp.cpp\
			authapi.pb.cc\
			authapi_service_common.cpp\
			authapi_service_client_evhttp.cpp\
			authapi_service_client_evhttp_main.cpp
#			authapi_service_sample_client_evhttp.cpp
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
	$(CC) $(CFLAGS) $(EVHTTP_SRCS) -o $(EVHTTP_TARGET) $(LIBS) $(INCLUDES) $(EVHTTP_LDFLAGS)

$(PROTOBUF_SRC):
	$(PROTOC) $(PROTOFLAGS) $(PROTOSRC)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(SAMPLECFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $(CLIENT_TARGET)

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(CLIENT_TARGET) 
