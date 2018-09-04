SRC=snafu
ENG=$(SRC)ENG
CC=gcc
CFLAGS=-O3 -Wall -Wextra
LDFLAGS=-I./$(ENG) -L. -l$(ENG)

all: clean build main

clean:
	sudo rm -f *.o *.so *.out

build:
	$(CC) $(CFLAGS) -c -fpic ./$(ENG)/$(ENG).c -I./$(ENG) -I./$(ENG)
	$(CC) $(CFLAGS) -shared -o lib$(ENG).so $(ENG).o

main:
	$(CC) $(CFLAGS) main.c -o $(SRC).out $(LDFLAGS)

run:
	LD_LIBRARY_PATH=. ./$(SRC).out
