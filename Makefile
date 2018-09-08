SRC=snafu
ENG:=$(SRC)ENG
CC=gcc
CFLAGS=-O3 -Wall -Wextra
LDFLAGS:=-I./$(ENG) -L. -l$(ENG)

all: clean build main rdebug wrlefile

clean:
	sudo rm -f *.o *.so *.out

build:
	$(CC) $(CFLAGS) -c -fpic ./$(ENG)/$(ENG).c -I./$(ENG)
	$(CC) $(CFLAGS) -shared -o lib$(ENG).so $(ENG).o

main:
	$(CC) $(CFLAGS) $@.c -o $(SRC).out $(LDFLAGS)

rdebug:
	$(CC) $(CFLAGS) $@.c -o $@.out $(LDFLAGS)

wrlefile:
	$(CC) $(CFLAGS) $@.c -o $@.out

run:
	LD_LIBRARY_PATH=. ./$(SRC).out

debug:
	LD_LIBRARY_PATH=. ./rdebug.out
