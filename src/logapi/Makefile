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
LDFLAGS = -lprotobuf -lbbrpc
PROTOC = $(BBHOME)/local/bin/protoc
PROTOFLAGS = --cpp_out=.
PROTOSRC = logapi.proto
PROTOBUF_SRC = logapi.pb.cc
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = logapiserver
SERVER_MAIN = logapi_service_main.cpp
SERVER_SRCS = $(COMMON_SRCS)\
				logapi_service.cpp\
				logapi_service_rpc.cpp\
				logapi_service_common.cpp
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o) 
SERVER_LDFLAGS = $(LDFLAGS) -lbbdrizzle -lapicommon

CLIENT_TARGET = logapisampleclient
CLIENT_MAIN = logapi_service_sample_client.cpp
CLIENT_SRCS = $(COMMON_SRCS)\
	      		logapi_service_common.cpp
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

BBEVQUEUE_LDFLAGS = $(LDFLAGS) -lbbevqueue
BBEVQUEUE_LIBS = $(LIBS) -L../libbbevqueue
BBEVQUEUE_INCLUDES = $(INCLUDES) -I../libbbevqueue
BBEVQUEUE_TARGET = logapibbevequeue
BBEVQUEUE_SRCS = logapi_service_client_bbevqueue_main.cpp\
				logapi_service_client_bbevqueue.cpp\
				logapi_service_common.cpp\
				logapi.pb.cc 
BBEVQUEUE_OBJS = $(BBEVQUEUE_SRCS:.cc=.o)

all: $(PROTOBUF_SRC) $(SERVER_TARGET) 

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(INCLUDES) $(SERVER_LDFLAGS) -o $(SERVER_TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

sample: $(PROTOBUF_TARGET) $(CLIENT_TARGET) $(BBEVQUEUE_TARGET)

$(BBEVQUEUE_TARGET):
	$(CC) $(CFLAGS) $(BBEVQUEUE_SRCS) -o $(BBEVQUEUE_TARGET) $(BBEVQUEUE_LIBS) $(BBEVQUEUE_INCLUDES) $(BBEVQUEUE_LDFLAGS)

$(PROTOBUF_SRC):
	$(PROTOC) $(PROTOFLAGS) $(PROTOSRC)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(SAMPLECFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $(CLIENT_TARGET)

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(CLIENT_TARGET) 
