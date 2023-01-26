CC=clang
CFLAGS=-g -Wall
SRC=src
OBJ=obj
BINDIR=bin
SRCS=$(wildcard $(SRC)/*c) 
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

TEST=tests
TESTS=$(wildcard $(TEST)/*.c)
TESTBINS=$(patsubst $(TEST)/%.c, $(TEST)/bin/%, $(TESTS))

BIN=$(BINDIR)/server

all: $(BIN)

release: CFLAGS=-Wall -O2 -DNDEBUG
release: clean
release: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

$(TEST)/bin/%: $(TEST)/*.c
	$(CC) $(CFLAGS) $^ $(OBJS) -o $@ -lcriterion

$(TEST)/bin:
	mkdir $@

$(OBJ):
	mkdir $@

$(BINDIR):
	mkdir $@

test: $(BIN) $(TEST)/bin $(TESTBINS)
	for test in $(TESTBINS) ; do ./$$test ; done

run:
	$(BIN)

clean:
	rm -rf bin/* obj/*