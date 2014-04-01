# Makefile for one dimensional cellular automation

CC = gcc
CFLAGS = -g -Wall -Wstrict-prototypes -ansi -pedantic

all: 1dcaArray 1dcaPointer

1dcaArray: 1dcaArray.o memcheck.o
	$(CC) 1dcaArray.o -o 1dcaArray

1dcaPointer: 1dcaPointer.o memcheck.o
	$(CC) 1dcaPointer.o -o 1dcaPointer

1dcaArray.o: 1dca.c memcheck.h
	$(CC) $(CFLAGS) -DPOINTER_VERSION=0 1dca.c -o 1dcaArray.o

1dcaPointer.o: 1dca.c memcheck.h
	$(CC) $(CFLAGS) -DPOINTER_VERSION=1 1dca.c -o 1dcaPointer.o

memcheck.o: memcheck.c memcheck.h
	$(CC) $(CFLAGS) -c memcheck.c

#currently 1dca.c defaults to using the array-index based solution
#1dcaArray.c 1dca.c:
#	cp 1dca.c 1dcaArray.c

#1dcaPointer.c 1dca.c:
#	patch --output=1dcaPointer.c --force 1dca.c pointer_patch

check:
	c_style_check 1dca.c

clean:
	rm -f *.rej *.o 1dcaArray 1dcaPointer
