.SUFFIXES: .cpp .o .h

CC = g++

COMMON_CFLAGS = -g -Wall -Werror -fprofile-arcs -ftest-coverage
OS	= ${shell uname}
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

LDFLAGS = -ldrizzle -lapicommon
LIBS = -L/usr/local/lib -L. $(BBLIBS) -L../libapicommon
INCLUDES = -I. -I../libapicommon $(BBINCLUDES)
TARGET = libbbdrizzle.so
OBJS = drizzle_client.o
MAINTARGET = drizzle_client_main
MAINOBJS = drizzle_client_main.o
MAINLDFLAGS = $(LDFLAGS) -lbbdrizzle

GCOVDATA = *.gcov *.gcda *.gcno *.info

LCOVFILE = libbbdrizzle.info
LCOVDIR = cov

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(OBJS) $(INCLUDES) $(LIBS) $(LDFLAGS) -o $(TARGET)

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LIBS) $<

clean:
	rm -f $(TARGET) $(OBJS) $(MAINTARGET) $(MAINOBJS)
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
