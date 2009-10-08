.SUFFIXES: .cc .cpp .o .h

CC = g++
COMMON_CFLAGS = -g -Wall -pipe
OS	= ${shell uname}
ifeq (${OS}, Linux)
CFLAGS = $(COMMON_CFLAGS) -fpic
CLIBFLAGS = -shared
endif
ifeq (${OS}, Darwin)
CFLAGS = $(COMMON_CFLAGS) -Kpic
CLIBFLAGS = -G -dynamiclib
endif

BBHOME = /usr/bb
BBINCLUDES = -I$(BBHOME)/local/include
BBLIBS = -L$(BBHOME)/local/lib

LIBLDFLAGS = -levent -lprotobuf
LIBS = -L/usr/local/lib -L. $(BBLIBS)
INCLUDES = -I. $(BBINCLUDES)
TARGET = libbbrpc.so
LIB_SRCS =  rpc_server.cpp\
			event_status.cpp\
			client_event_status.cpp\
			rpccommon.cpp\
			bb_rpc_channel.cpp\
			bb_rpc_controller.cpp\
			bb_rpc_service.cpp
LIB_OBJS = $(LIB_SRCS:.cpp=.o)

SAMPLECFLAGS = $(COMMON_CFLAGS)
LDFLAGS = -lprotobuf -lbbrpc
PROTOC = protoc
PROTOFLAGS = --cpp_out=.
PROTOSRC = sample.proto
PROTOBUF_SRC = sample.pb.cc
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = server
SERVER_MAIN = sample_rpc_service.cpp
SERVER_SRCS = $(COMMON_SRCS)
SERVER_OBJS = $(SERVER_SRCS:.cc=.o) 

CLIENT_TARGET = client
CLIENT_MAIN = sample_rpc_client.cpp
CLIENT_SRCS = $(COMMON_SRCS)
CLIENT_OBJS = $(CLIENT_SRCS:.cc=.o)

EVHTTP_TARGET = evhttp_client
EVHTTP_SRCS = bb_rpc_client_evhttp.cpp\
			sample_rpc_client_evhttp_main.cpp
EVHTTP_OBJS = $(EVHTTP_SRCS:.cc=.o)

all: $(TARGET)

$(TARGET): $(LIB_OBJS)
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(LIB_OBJS) $(LIBS) $(LIBLDFLAGS) -o $(TARGET)

sample: $(PROTOC_TARGET) $(SERVER_TARGET) $(CLIENT_TARGET) $(EVHTTP_TARGET)

$(PROTOC_TARGET):
	$(PROTOC) $(PROTOFLAGS) $(PROTOSRC)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(SAMPLECFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(LDFLAGS) -o $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(SAMPLECFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(LDFLAGS) -o $(CLIENT_TARGET)

$(EVHTTP_TARGET): $(EVHTTP_OBJS)
	$(CC) $(SAMPLECFLAGS) $(EVHTTP_OBJS) -o $(EVHTTP_TARGET) -levent

.cpp.o:
	$(CC) -c $(CFLAGS) $< $(LIBS) $(INCLUDES)

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(TARGET) $(LIB_OBJS) $(PROTOSRC)

install: $(TARGET)
	mkdir -p /usr/bb/lib
	install -m644 $(TARGET) /usr/bb/lib
