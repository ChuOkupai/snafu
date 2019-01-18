ENG = snafuENG
GAME = snafu
CC = gcc
CFLAGS = -O3 -Wall -Wextra -Werror -I./include -I./$(ENG)/include
LDFLAGS = -L./lib -l$(ENG)
VERSION = $(shell cat include/snafu.h | grep Version | cut -b 13-18)

clean:
	make -C $(ENG) clean
	rm -f *.debug *.log *.out *.zip
	rm -rf bin lib $(GAME)

bin/$(GAME).out: src/snafu.c
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

build:
	make -C $(ENG) lib$(ENG).so
	mkdir -p bin lib
	cp $(ENG)/lib$(ENG).so lib
	make bin/$(GAME).out

run: build
	LD_LIBRARY_PATH=./lib ./bin/$(GAME).out

export: build
	mkdir $(GAME)
	cp assets/launch.sh $(GAME)
	cp assets/README $(GAME)
	mv bin $(GAME)
	cp -r data $(GAME)
	cp $(ENG)/LICENSE lib
	mv lib $(GAME)
	cp LICENSE $(GAME)
	echo $(VERSION) > $(GAME)/version
	zip -r $(GAME)-$(VERSION).zip $(GAME)
	rm -r $(GAME)