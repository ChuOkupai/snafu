ENG=snafuENG
GAME=snafu

clean:
	rm -f *.out
	cd $(ENG) && make clean
	cd $(GAME) && make clean

update: wrlef.out $(GAME).out

all: clean update

lib$(ENG).so:
	cd $(ENG) && make $@

wrlef.out:
	cd $(ENG) && make $@ && cp wrlef.out ..

exportlib: lib$(ENG).so
	mkdir -p snafu/bin
	cd $(ENG) && cp $< ../snafu/bin

$(GAME).out: exportlib
	cd $(GAME) && make $@

run: snafu.out
	cd $(GAME) && make run
