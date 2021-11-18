bin:
	mkdir bin
	cd bin && cmake ../src
	cd bin && make
install:	bin
	cd bin && make install

all:	bin
