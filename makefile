CC=gcc
CFLAGS= -Wall -pedantic -g
LDLIBS= -lm


.PHONY: run
run: clean test_unbounded
	./out/test_unbounded

test_unbounded : test_unbounded.c unbounded_int.c
	$(CC) $(CFLAGS) test_unbounded.c -o out/test_unbounded

clean :
	rm -rd out/*