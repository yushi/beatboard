CC=g++ -Wall -g
CC_OPTS=-I /home/bb/local/include/ -L /home/bb/local/lib -L ./
OBJS=bb_protobuf_queue_memcached.o bb_ircd_event_queue.o bb_queue_index.pb.o bb_ircd_event.pb.o
SOURCES=bb_protobuf_queue_memcached.cpp bb_ircd_event_queue.cpp bb_queue_index.pb.cc bb_ircd_event.pb.cc
LIBS=-lmemcached -lprotobuf -lbbevqueue

all: client cas_test
	LD_LIBRARY_PATH=. ./client
	LD_LIBRARY_PATH=. ./cas_test

bb_queue_index.pb.h:  bb_queue_index.proto
	protoc -I=. --cpp_out=. bb_queue_index.proto

bb_ircd_event.pb.h: bb_ircd_event.proto
	protoc -I=. --cpp_out=. bb_ircd_event.proto

bb_queue_index.pb.cc:  bb_queue_index.proto
	protoc -I=. --cpp_out=. bb_queue_index.proto

bb_ircd_event.pb.cc: bb_ircd_event.proto
	protoc -I=. --cpp_out=. bb_ircd_event.proto

bb_queue_index.pb.o:  bb_queue_index.pb.cc
	$(CC) $(CC_OPTS) -c $<

bb_ircd_event.pb.o:  bb_ircd_event.pb.cc
	$(CC) $(CC_OPTS) -c $<

.cpp.o: 
	$(CC) $(CC_OPTS) -c $<

.cc.o: 
	$(CC) $(CC_OPTS) -c $<

client: memd_client.cpp libbbevqueue.so
	$(CC) $(CC_OPTS) memd_client.cpp -o $@ $(LIBS)

cas_test: memd_cas_test.cpp libbbevqueue.so
	$(CC) $(CC_OPTS) memd_cas_test.cpp -o $@ $(LIBS) -lpthread 

libbbevqueue.so: $(SOURCES) $(OBJS)
	$(CC) $(CC_OPTS) $(OBJS) --shared -o libbbevqueue.so
clean:
	rm -rf *.o
	rm -rf ./cas_test
	rm -rf ./client
	rm -rf bb_queue_index.pb.cc
	rm -rf bb_queue_index.pb.h
