CC=g++ -Wall -g
CC_OPTS=-I /home/bb/local/include/ -L /home/bb/local/lib
OBJS=bb_protobuf_queue_memcached.o bb_queue_index.pb.o
LIBS=-lmemcached -lprotobuf
.cpp.o:
	$(CC) $(CC_OPTS) -c $<

all: protobuf client init
	./init
	./client

protobuf:
	protoc -I=. --cpp_out=. bb_queue_index.proto
	$(CC) $(CC_OPTS) -c bb_queue_index.pb.cc
	
client: memd_client.o $(OBJS)
	$(CC) $(CC_OPTS) $^ -o $@ $(LIBS)

init: memd_initializer.o $(OBJS)
	$(CC) $(CC_OPTS) $^ -o $@ $(LIBS)

clean:
	rm -rf *.o
	rm -rf ./init
	rm -rf ./client
	rm -rf bb_queue_index.pb.cc
	rm -rf bb_queue_index.pb.h
