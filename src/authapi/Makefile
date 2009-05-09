.SUFFIXES: .cc .cpp .o .h

CC = g++
CFLAGS = -g -Wall -pipe
OS	= ${shell uname}

LIBLDFLAGS = -lprotobuf -lbbrpc -luuid
LIBS = -L/usr/local/lib -L. -L../libbbrpc/ -L../libbbdrizzle/ -L../libapicommon
INCLUDES = -I../libbbrpc/ -I. -I../libbbdrizzle/ -I../libapicommon

SAMPLECFLAGS = $(COMMON_CFLAGS)
LDFLAGS = -lprotobuf -lbbrpc
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

all: $(PROTOBUF_SRC) $(SERVER_TARGET) 

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(INCLUDES) $(SERVER_LDFLAGS) -o $(SERVER_TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

.cc.o:
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -c $<

sample: $(PROTOBUF_TARGET) $(CLIENT_TARGET)

$(PROTOBUF_SRC):
	$(PROTOC) $(PROTOFLAGS) $(PROTOSRC)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(SAMPLECFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $(CLIENT_TARGET)

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(CLIENT_TARGET) 