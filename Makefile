CC=gcc
CFLAGS=-Ofast -Wall -std=gnu11
SAN=-fsanitize=undefined -fsanitize=address

.PHONY: all clean

all: test_pril

test_pril.o: test_pril.c pril.h
	$(CC) $(CFLAGS) $(SAN) $< -c -I. -o $@

test_pril: test_pril.o
	$(CC) $(CFLAGS) $(SAN) $^ -o $@


clean:
	rm -f *.o test_pril
