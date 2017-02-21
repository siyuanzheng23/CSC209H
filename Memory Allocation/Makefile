CC=gcc
CFLAGS=-Wall -g
LDFLAGS=

OBJS=heap209.o diagnostics.o

all: test-basic1

test-basic1: test-basic1.o $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c heap209.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f test-basic1 *.o

test-c3zhengt: test-c3zhengt.c
	$(CC) $(CFLAGS) -c test-c3zhengt.c -o test-c3zhengt



test: test-c3zhengt.c test-basic1.c
	$(CC) $(CFLAGS) -c test-c3zhengt.c test-basic1.c -o test

