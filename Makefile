build:
	make -C main

install: libdusk.so
	sh install.sh

tests:
	@for d in $(wildcard test/*/); do make build -C $$d; done

clean:
	make -C main clean
	@for d in $(wildcard test/*/); do make clean -C $$d; done
