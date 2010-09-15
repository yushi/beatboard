CLEANFILES = sample.pb.h sample.pb.cc

lib_LTLIBRARIES = libbbrpc.la
libbbrpc_la_SOURCES = rpc_server.cpp \
					  rpc_server.h \
					  event_status.cpp \
					  event_status.h \
					  client_event_status.cpp\
					  client_event_status.h\
					  rpccommon.cpp \
					  rpccommon.h \
					  bb_rpc_channel.cpp \
					  bb_rpc_channel.h \
					  bb_rpc_controller.cpp \
					  bb_rpc_controller.h \
					  bb_rpc_service.cpp \
					  bb_rpc_service.h
libbbrpc_la_LIBADD = -levent