.SUFFIXES: .cc .cpp .o .h

CC = g++
CFLAGS = -g -Wall -pipe
OS	= ${shell uname}

LIBLDFLAGS = -lprotobuf -lbbrpc
LIBS = -L/usr/local/lib -L. -L../libbbrpc/
INCLUDES = -I../libbbrpc/ -I.

SAMPLECFLAGS = $(COMMON_CFLAGS)
LDFLAGS = -lprotobuf -lbbrpc
PROTOC = protoc
PROTOFLAGS = --cpp_out=.
PROTOSRC = logapi.proto
PROTOBUF_SRC = logapi.pb.cc
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = logapiserver
SERVER_MAIN = logapi_service_main.cpp
SERVER_SRCS = $(COMMON_SRCS)\
				logapi_service.cpp\
				logapi_service_rpc.cpp
SERVER_OBJS = $(SERVER_SRCS:.cc=.o) 

CLIENT_TARGET = client
CLIENT_MAIN = sample_rpc_client.cpp
CLIENT_SRCS = $(COMMON_SRCS)
CLIENT_OBJS = $(CLIENT_SRCS:.cc=.o)

all: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $(SERVER_TARGET)

sample: $(PROTOC_TARGET) $(CLIENT_TARGET)

$(PROTOC_TARGET):
	$(PROTOC) $(PROTOFLAGS) $(PROTOSRC)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(SAMPLECFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(INCLUDES) $(LDFLAGS) -o $(CLIENT_TARGET)

.cpp.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(TARGET) $(LIB_OBJS) $(PROTOSRC)
