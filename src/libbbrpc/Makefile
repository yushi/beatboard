.SUFFIXES: .cpp .o .h

CC = g++
CFLAGS = -Wall -Werror -pipe
CLIBFLAGS = -G -dynamiclib -Kpic
LIBLDFLAGS = -levent -lprotobuf
LIBS = -L/usr/local/lib -L.
INCLUDES = -I.
TARGET = libbbrpc.so
LIB_SRCS =  rpc_server.cpp\
			event_status.cpp\
			client_event_status.cpp\
			rpccommon.cpp\
			bb_rpc_channel.cpp\
			bb_rpc_controller.cpp\
			bb_rpc_service.cpp
LIB_OBJS = $(LIB_SRCS:.cpp=.o)

LDFLAGS = -lprotobuf -lbbrpc
PROTOBUF_SRC = sample.pb.cc
COMMON_SRCS = $(PROTOBUF_SRC)

SERVER_TARGET = server
SERVER_MAIN = sample_rpc_service.cpp
SERVER_SRCS = $(COMMON_SRCS)
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)

CLIENT_TARGET = client
CLIENT_MAIN = sample_rpc_client.cpp
CLIENT_SRCS = $(COMMON_SRCS)
CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(LIB_OBJS)
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(LIB_OBJS) $(LIBLDFLAGS) -o $(TARGET)

sample: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CC) $(CFLAGS) $(SERVER_MAIN) $(SERVER_OBJS) $(LIBS) $(LDFLAGS) -o $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) $(CLIENT_MAIN) $(CLIENT_OBJS) $(LIBS) $(LDFLAGS) -o $(CLIENT_TARGET)

.cpp.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(SERVER_TARGET) $(SERVER_OBJS) $(CLIENT_TARGET) $(CLIENT_OBJS) $(TARGET) $(LIB_OBJS)

