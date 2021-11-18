bin:
	mkdir bin
	cd bin && cmake ../
	cd bin && make
install:	bin
	cd bin && make install

all:	bin
