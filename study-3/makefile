CXXFLAGS = -Wall -g -std=c++11 -Wc++11-extensions

extempore-analysis: code-tree.o code-history.o code-diff.o code-graph.o
	g++ code-tree.o code-history.o code-diff.o code-graph.o -dynamiclib -o code.dylib

test: code-tree.o code-history.o code-diff.o code-graph.o
	g++ code-tree.o code-history.o code-diff.o code-graph.o -o code-test
	./code-test

code-tree.o:
	g++ $(CXXFLAGS) -c -o code-tree.o code-tree.cpp

code-history.o:
	g++ $(CXXFLAGS) -c -o code-history.o code-history.cpp

code-diff.o:
	g++ $(CXXFLAGS) -c -o code-diff.o code-diff.cpp

code-graph.o:
	g++ $(CXXFLAGS) -c -o code-graph.o code-graph.cpp

clean:
	rm -f *.o