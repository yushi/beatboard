.SUFFIXES: .cpp .o .h

CC = g++

COMMON_CFLAGS = -Wall -Werror
OS	= ${shell uname}
ifeq (${OS}, Linux)
CFLAGS = $(COMMON_CFLAGS) -fpic
CLIBFLAGS = -shared
endif
ifeq (${OS}, Darwin)
CFLAGS = $(COMMON_CFLAGS) -Kpic
CLIBFLAGS = -G -dynamiclib
endif

LDFLAGS = 
LIBS = -L/usr/local/lib -L.
INCLUDES = -I.
TARGET = libapicommon.so
OBJS = api_common.o
MAINTARGET = api_common_main
MAINOBJS = api_common_main.o
MAINLDFLAGS = $(LDFLAGS) -lapicommon

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(OBJS) $(LDFLAGS) -o $(TARGET)

.cpp.o:
	$(CC) -c $(CFLAGS) $<

clean:
	rm -f $(TARGET) $(OBJS) $(MAINTARGET) $(MAINOBJS)

main: $(MAINTARGET)

$(MAINTARGET): $(MAINOBJS) $(TARGET)
	$(CC) $(CFLAGS) $(LIBS) $(MAINLDFLAGS) $(MAINOBJS) -o $(MAINTARGET)