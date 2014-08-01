CXXFLAGS = -Wall -g -std=c++11 -Wc++11-extensions

all:
	$(MAKE) clean
	$(MAKE) extempore-analysis

extempore-analysis: bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o
	g++ bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o -dynamiclib -o bin/code.dylib

test: bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o bin/main.o
	g++ bin/code-tree.o bin/code-history.o bin/code-diff.o bin/code-graph.o bin/main.o -o bin/code-test
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

clean:
	rm -f ./bin/*.o
	rm -f ./bin/code.dylib
	rm -f ./bin/code-test