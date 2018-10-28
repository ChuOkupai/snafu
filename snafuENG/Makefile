ENG=snafuENG
CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror

clean:
	rm -f *.o *.so *.out

$(ENG).o: $(ENG).c $(ENG).h
	$(CC) $(CFLAGS) -c -fpic $< -o $@ -I.

lib$(ENG).so: $(ENG).o
	$(CC) $(CFLAGS) -shared $< -o $@

wrlef.out: $(ENG)-wrlef.c
	$(CC) $(CFLAGS) $< -o $@
