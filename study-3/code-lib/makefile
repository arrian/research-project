CXXFLAGS = -Wall -g -std=c++11 -Wc++11-extensions

all:
	$(MAKE) clean
	$(MAKE) code-lib

code-lib:
	g++ $(CXXFLAGS) code-manager.cpp event-queue.cpp -dynamiclib -o ./bin/code.dylib

test:
	g++ $(CXXFLAGS) code-manager.cpp event-queue.cpp -o ./bin/code-test

clean:
	rm -f ./bin/*.o
	rm -f ./bin/code.dylib
	rm -f ./bin/code-test