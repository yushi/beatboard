CC=g++ -Wall -g
CC_OPTS=-I /home/bb/local/include/ -L /home/bb/local/lib
OBJS=bb_protobuf_queue_memcached.o bb_queue_index.pb.o
SOURCES=bb_protobuf_queue_memcached.cpp bb_queue_index.pb.cc
LIBS=-lmemcached -lprotobuf

all: client cas_test
	./client
	./cas_test

bb_queue_index.pb.h:  bb_queue_index.proto
	protoc -I=. --cpp_out=. bb_queue_index.proto

bb_queue_index.pb.cc:  bb_queue_index.proto
	protoc -I=. --cpp_out=. bb_queue_index.proto

bb_queue_index.pb.o:  bb_queue_index.pb.cc
	$(CC) $(CC_OPTS) -c $<

.cpp.o: 
	$(CC) $(CC_OPTS) -c $<

.cc.o: 
	$(CC) $(CC_OPTS) -c $<

client: $(SOURCES) $(OBJS) memd_client.cpp
	$(CC) $(CC_OPTS) memd_client.cpp $(OBJS) -o $@ $(LIBS)

cas_test: $(SOURCES) $(OBJS) memd_cas_test.cpp
	$(CC) $(CC_OPTS) memd_cas_test.cpp $(OBJS) -o $@ $(LIBS) -lpthread

clean:
	rm -rf *.o
	rm -rf ./cas_test
	rm -rf ./client
	rm -rf bb_queue_index.pb.cc
	rm -rf bb_queue_index.pb.h
