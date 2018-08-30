SRC=snafu
CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-I. -L. -l$(SRC)ENG

all: clean build main

clean:
	sudo rm -f *.o *.so *.out

build:
	$(CC) $(CFLAGS) -c -fpic $(SRC)ENG.c -I.
	$(CC) $(CFLAGS) -shared -o lib$(SRC)ENG.so $(SRC)ENG.o

main:
	$(CC) $(CFLAGS) main.c -o $(SRC).out $(LDFLAGS)

run:
	LD_LIBRARY_PATH=. ./$(SRC).out
