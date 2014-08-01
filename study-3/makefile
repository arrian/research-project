CXXFLAGS = -Wall -g -std=c++11 -Wc++11-extensions

all:
	code-lib
	app-lib

clean:
	app-lib-clean
	code-lib-clean

app-lib:
	cd app-lib;make all

app-lib-clean:
	cd app-lib;make clean

code-lib:
	cd code-lib;make all

code-lib-clean:
	cd code-lib;make clean

test:
	cd test;make
