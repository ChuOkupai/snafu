ENG=snafuENG
GAME=snafu
CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror
LDFLAGS:=-I../$(ENG) -L./bin -l$(ENG)

clean:
	rm -f *.debug *.log *.out bin/*

$(GAME).out: main.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: $(GAME).out
	LD_LIBRARY_PATH=./bin ./$(GAME).out
