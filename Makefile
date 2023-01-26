CC=clang
CFLAGS=-g -Wall
SRC=src
OBJ=obj
BINDIR=bin
SRCS=$(wildcard $(SRC)/*c) 
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

BIN=$(BINDIR)/server

all: $(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

run:
	$(BIN)

clean:
	rm -rf bin/* obj/*