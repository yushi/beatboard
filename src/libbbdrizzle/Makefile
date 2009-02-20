.SUFFIXES: .cpp .o .h

CC = g++
CFLAGS = -Wall -Werror
CLIBFLAGS = -G -dynamiclib
LDFLAGS = -ldrizzle
LIBS = -L/usr/local/lib -L.
INCLUDES = -I.
TARGET = libbbdrizzle.so
OBJS = drizzle_client.o
MAINTARGET = drizzle_client_main
MAINOBJS = drizzle_client_main.o
MAINLDFLAGS = $(LDFLAGS) -lbbdrizzle

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
