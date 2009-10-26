.SUFFIXES: .cc .o .h

OS = ${shell uname}
ifeq (${OS}, Linux)
ENV_LIBRARY_PATH=LD_LIBRARY_PATH
endif
ifeq (${OS}, Darwin)
ENV_LIBRARY_PATH=DYLD_LIBRARY_PATH
endif

BB_HOME=/usr/bb
CC = g++
GCOVFLAGS = -fprofile-arcs -ftest-coverage
CFLAGS = -g3 -Wall $(GCOVFLAGS)
LDFLAGS = -lapicommon -lbbdrizzle -lgtest
LIBS = -L.. -L../../libapicommon -L$(BB_HOME)/local/lib -L/usr/local/lib
INCLUDES = -I.. -I../../libapicommon -I$(BB_HOME)/local/include -I/usr/local/include
TARGET = test_bbdrizzle
SRCS = test_bbdrizzle.cpp
OBJS = $(SRCS:.cpp=.o)
GCOVDATA = *.gcov *.gcda *.gcno *.info


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(CLIBFLAGS) $(LIBS) $(OBJS) $(INCLUDES) $(LDFLAGS) -o $(TARGET)

.cpp.o:
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LIBS) $<

clean:
	rm -f $(TARGET) $(OBJS) $(GCOVDATA)

test: clean $(TARGET)
	$(ENV_LIBRARY_PATH)=..:../../libapicommon:$(BB_HOME)/local/lib:/usr/local/lib ./$(TARGET)
