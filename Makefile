CC=clang
CFLAGS=-g -Wall
BINS=server
OBJS=server.o myqueue.o error.o

all: $(BINS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^

server: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -rf rm*.o *.dSYM $(BINS)