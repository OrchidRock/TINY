.PHONY: clean

all: libcsapp.so tiny

libcsapp.so:
	make -C ./lib || exit $$?

tiny:  
	make -C ./src || exit $$?


clean:
	make -C ./lib clean
	make -C ./src clean
