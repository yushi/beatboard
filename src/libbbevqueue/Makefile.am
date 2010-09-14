lib_LTLIBRARIES = libbbevqueue.la
libbbevqueue_la_SOURCES = bb_ircd_event_queue.cpp bb_protobuf_queue_memcached.cpp
CLEANFILES= bb_queue_index.pb.cc bb_queue_index.pb.h bb_ircd_event.pb.cc bb_ircd_event.pb.h

bb_ircd_event_queue.cpp: bb_queue_index.pb.cc
bb_ircd_event_queue.cpp: bb_ircd_event.pb.cc

bb_ircd_event.pb.cc: bb_ircd_event.proto
	@PROTOC@ -I=. --cpp_out=. $<	

bb_queue_index.pb.cc: bb_queue_index.proto
	@PROTOC@ -I=. --cpp_out=. $<

