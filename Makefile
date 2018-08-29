SRC=snafu
CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-I. -L. -l$(SRC)

all: clean build main

clean:
	sudo rm -f *.o *.so *.out

build:
	$(CC) $(CFLAGS) -c -fpic $(SRC).c -I.
	$(CC) $(CFLAGS) -shared -o lib$(SRC).so $(SRC).o

main:
	$(CC) $(CFLAGS) main.c -o $(SRC).out $(LDFLAGS)

run:
	LD_LIBRARY_PATH=. ./$(SRC).out
