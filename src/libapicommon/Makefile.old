.SUFFIXES: .cpp .o .h

CC = g++

COMMON_CFLAGS = -g -Wall -Werror -fprofile-arcs -ftest-coverage
OS = ${shell uname}
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

LDFLAGS = -lmemcached
LIBS = -L/usr/local/lib -L. $(BBLIBS)
INCLUDES = -I. $(BBINCLUDES)
TARGET = libapicommon.so
SRCS = api_common.cpp\
		api_memcached.cpp
OBJS = $(SRCS:.cpp=.o)
MAINTARGET = api_common_main
MAINOBJS = api_common_main.o
MAINLDFLAGS = $(LDFLAGS) -lapicommon
GCOVDATA = *.gcov *.gcda *.gcno *.info

LCOVFILE = libapicommon.info
LCOVDIR = cov

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(LIBS) $(OBJS) $(INCLUDES) $(LDFLAGS) -o $(TARGET)

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LIBS) $<

clean:
	rm -f $(TARGET) $(OBJS) $(MAINTARGET) $(MAINOBJS) $(GCOVDATA)
	if test -d $(LCOVDIR); then rm -r $(LCOVDIR); fi;
	make -C tests clean

main: $(MAINTARGET)

$(MAINTARGET): $(MAINOBJS) $(TARGET)
	$(CC) $(CFLAGS) $(LIBS) $(MAINLDFLAGS) $(MAINOBJS) -o $(MAINTARGET)

install: $(TARGET)
	mkdir -p /usr/bb/lib
	install -m644 $(TARGET) /usr/bb/lib

test: clean all
	make -C tests test
	lcov -c --output-file $(LCOVFILE) -d .
	genhtml $(LCOVFILE) -o $(LCOVDIR)
