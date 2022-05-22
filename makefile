CC=gcc
CFLAGS= -Wall -pedantic -g
LDLIBS= -lm


.PHONY: clean
run_calc: clean calc_unbounded

run_test: clean test_unbounded

test_unbounded :
	$(CC) $(CFLAGS) test_unbounded.c unbounded_int.c -o out/test_unbounded

calc_unbounded :
	$(CC) $(CFLAGS) calc_unbounded_int.c unbounded_int.c -o out/calc_unbounded_int

clean :
	rm -rdf out/
	mkdir out/
