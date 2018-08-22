CC = gcc
CFLAGS = -Wall -g -m32
 
all: memgrind.c mymalloc.o
	$(CC) $(CFLAGS) -o memgrind memgrind.c mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	$(CC) $(CFLAGS) -c mymalloc.c
 
clean: 
	rm -rf *.o mymalloc memgrind
