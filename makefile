CC=gcc
CFLAGS= -Wall -pedantic -g
LDLIBS= -lm


.PHONY: run
run_test: clean test_unbounded

run_calc: clean calc_unbounded

test_unbounded : test_unbounded.c unbounded_int.c
	$(CC) $(CFLAGS) test_unbounded.c -o out/test_unbounded

calc_unbounded : calc_unbounded_int.c unbounded_int.c
	$(CC) $(CFLAGS) calc_unbounded_int.c -o out/calc_unbounded_int

clean :
	rm -r out/*