CC=gcc
CFLAGS=-I. -g -Wall
DEPS = wish.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

wish: wish.o wish_fkt.o 
	$(CC) -o wish wish.o wish_fkt.o 
