# Makefile for quicksorter program

CC = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

quicksorter: quicksorter.o linked_list.o memcheck.o
	$(CC) quicksorter.o linked_list.o memcheck.o -o quicksorter

quicksorter.o: quicksorter.c linked_list.h memcheck.h
	$(CC) $(CFLAGS) -c quicksorter.c

linked_list.o: linked_list.c linked_list.h
	$(CC) $(CFLAGS) -c linked_list.c

memcheck.o: memcheck.c memcheck.h
	$(CC) $(CFLAGS) -c memcheck.c

check:
	c_style_check *.c

test:
	./test.sh

clean:
	rm -f *.rej *.o quicksorter
