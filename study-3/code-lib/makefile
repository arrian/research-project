CXXFLAGS = -Wall -g -std=c++11 -Wc++11-extensions

all:
	$(MAKE) clean
	$(MAKE) code-lib

code-lib: bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o bin/event-queue.o
	g++ bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o bin/event-queue.o -dynamiclib -o bin/code.dylib

code-simple-lib:
	g++ $(CXXFLAGS) code-simple-graph.cpp code-manager.cpp code-line.cpp code-tree.cpp list.cpp event-queue.cpp -dynamiclib -o ./bin/code.dylib

test: bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o bin/event-queue.o bin/main.o
	g++ bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o bin/event-queue.o bin/main.o -o bin/code-test
	./bin/code-test

bin/main.o:
	g++ $(CXXFLAGS) -c -o bin/main.o main.cpp

bin/code-tree.o:
	g++ $(CXXFLAGS) -c -o bin/code-tree.o code-tree.cpp

bin/code-history.o:
	g++ $(CXXFLAGS) -c -o bin/code-history.o code-history.cpp

bin/code-diff.o:
	g++ $(CXXFLAGS) -c -o bin/code-diff.o code-diff.cpp

bin/code-graph.o:
	g++ $(CXXFLAGS) -c -o bin/code-graph.o code-graph.cpp

bin/event-queue.o:
	g++ $(CXXFLAGS) -c -o bin/event-queue.o event-queue.cpp

clean:
	rm -f ./bin/*.o
	rm -f ./bin/code.dylib
	rm -f ./bin/code-test