CC ?= gcc
CFLAGS ?= -Wall -Wno-deprecated-declarations -g

test_conio: test_conio.c ../conio.c ../conio.h
	$(CC) $(CFLAGS) -I.. test_conio.c ../conio.c -o $@ -lncurses -lutil

.PHONY: test clean
test: test_conio
	./test_conio

clean:
	rm -f test_conio
