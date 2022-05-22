CC=gcc
CFLAGS= -Wall -pedantic -g


calc_unbounded_int : clean calc_unbounded_int.o unbounded_int.o
	$(CC) out/calc_unbounded_int.o out/unbounded_int.o -o out/calc_unbounded_int

test_unbounded : clean test_unbounded.o unbounded_int.o
	$(CC) out/test_unbounded.o out/unbounded_int.o -o out/test_unbounded

calc_unbounded_int.o : calc_unbounded_int.c unbounded_int.h
	$(CC) $(CFLAGS) -c calc_unbounded_int.c -o out/calc_unbounded_int.o

test_unbounded.o : test_unbounded.c unbounded_int.h
	$(CC) $(CFLAGS) -c test_unbounded.c -o out/test_unbounded.o

unbounded_int.o : unbounded_int.c unbounded_int.h
	$(CC) $(CFLAGS) -c unbounded_int.c -o out/unbounded_int.o

clean :
	rm -rdf out/
	mkdir out/
