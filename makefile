
.PHONY: run
run: test_unbounded
	./out/test_unbounded

test_unbounded : unbounded_int.h unbounded_int.c test_unbounded.c
	gcc -Wall test_unbounded.c -o out/test_unbounded