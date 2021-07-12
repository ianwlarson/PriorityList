CC=gcc
CFLAGS=-Ofast -Wall -std=gnu11 -ggdb3
SAN=-fsanitize=undefined -fsanitize=address

.PHONY: all clean

all: test_pril test_pril_fuzz test_bench

test_helper.o: test_helper.c test_helper.h pril.h
	$(CC) $(CFLAGS) $(SAN) $< -c -o $@

test_pril.o: test_pril.c test_helper.h
	$(CC) $(CFLAGS) $(SAN) $< -c -o $@

test_pril: test_pril.o test_helper.o
	$(CC) $(CFLAGS) $(SAN) $^ -o $@

test_pril_fuzz.o: test_pril_fuzz.c test_helper.h
	$(CC) $(CFLAGS) $(SAN) $< -c -o $@

test_pril_fuzz: test_pril_fuzz.o test_helper.o
	$(CC) $(CFLAGS) $(SAN) $^ -o $@

test_bench: test_bench.c test_helper.c
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f *.o test_pril test_pril_fuzz test_bench
