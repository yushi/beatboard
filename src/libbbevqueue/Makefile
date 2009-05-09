OS      = ${shell uname}
ifeq (${OS}, Linux)
CLIBFLAGS=-shared
LIBBBEVQUEUE=libbbevqueue.so
ENV_LIBRARY_PATH=LD_LIBRARY_PATH
endif
ifeq (${OS}, Darwin)
CLIBFLAGS=-dynamiclib
LIBBBEVQUEUE=libbbevqueue.dylib
ENV_LIBRARY_PATH=DYLD_LIBRARY_PATH
endif

PROTOC=/bb/local/bin/protoc
CC=g++ -Wall -g
CC_OPTS=-I /bb/local/include/ -L/bb/local/lib -L./
OBJS=bb_protobuf_queue_memcached.o bb_ircd_event_queue.o bb_queue_index.pb.o bb_ircd_event.pb.o
SOURCES=bb_protobuf_queue_memcached.cpp bb_ircd_event_queue.cpp bb_queue_index.pb.cc bb_ircd_event.pb.cc
LIBS=-lmemcached -lprotobuf

all: client cas_test
	$(ENV_LIBRARY_PATH)=. ./client
	$(ENV_LIBRARY_PATH)=. ./cas_test

bb_queue_index.pb.h:  bb_queue_index.proto
	$(PROTOC) -I=. --cpp_out=. bb_queue_index.proto

bb_ircd_event.pb.h: bb_ircd_event.proto
	$(PROTOC) -I=. --cpp_out=. bb_ircd_event.proto

bb_queue_index.pb.cc:  bb_queue_index.proto
	$(PROTOC) -I=. --cpp_out=. bb_queue_index.proto

bb_ircd_event.pb.cc: bb_ircd_event.proto
	$(PROTOC) -I=. --cpp_out=. bb_ircd_event.proto

bb_queue_index.pb.o:  bb_queue_index.pb.cc
	$(CC) $(CC_OPTS) -c $<

bb_ircd_event.pb.o:  bb_ircd_event.pb.cc
	$(CC) $(CC_OPTS) -c $<

.cpp.o: 
	$(CC) $(CC_OPTS) -c $<

.cc.o: 
	$(CC) $(CC_OPTS) -c $<

client: memd_client.cpp $(LIBBBEVQUEUE)
	$(CC) $(CC_OPTS) memd_client.cpp -o $@ -lbbevqueue #$(LIBS)

cas_test: memd_cas_test.cpp $(LIBBBEVQUEUE)
	$(CC) $(CC_OPTS) memd_cas_test.cpp -o $@ -lpthread -lbbevqueue #$(LIBS) 

$(LIBBBEVQUEUE): $(SOURCES) $(OBJS)
	$(CC) $(CC_OPTS) $(OBJS) $(CLIBFLAGS) $(LIBS) -o $(LIBBBEVQUEUE)
clean:
	rm -rf *.o
	rm -rf ./cas_test ./cas_test.dSYM 
	rm -rf ./client ./client.dSYM
	rm -rf bb_queue_index.pb.cc
	rm -rf bb_ircd_event.pb.cc
	rm -rf bb_queue_index.pb.h
	rm -rf bb_ircd_event.pb.h
	rm -rf $(LIBBBEVQUEUE)