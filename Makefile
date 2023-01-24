CC=clang

CFLAGS=-g
COMMON_OBJS = common.o

all: $(COMMON_OBJS) tcps

common.o: common.h common.c
	$(CC) $(CFLAGS) -c common.c

tcps: tcpserver.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) -o tcps tcpserver.c $(COMMON_OBJS)


clean:
	rm -rf *.o *.dSYM tcps