
#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <streambuf>
#include <vector>
#include <stack>
#include <locale>
#include <utility>
#include <cmath>

#include "code-tree.h"

class CodeGraph
{
public:
	CodeGraph(CodeTree* code, CodeGraph* parent = nullptr);
	~CodeGraph();

	void update(CodeTree* code);
	void step(float dt);

	void addChild(CodeTree* code);
	
	bool isMatch(CodeTree* code);

	float x;
	float y;
	float size;
	float vx;
	float vy;

	float tx;//temp location
	float ty;//temp location

	int timeAdded;
	int timeActivated;
	bool active;

	CodeGraph* parent;
	std::vector<CodeGraph*> children;

	std::string tag;
	CodeType type;
	std::string code;
	//CodeTree* data;

	void print(int depth = 0);

	std::vector<CodeGraph*> getVertices();
	std::vector<std::pair<CodeGraph*, CodeGraph*> > getEdges();
};

// int main()
// {
// 	CodeTree* tree = new CodeTree(load("test.xtm"));
// 	tree->parse();
// 	tree->printTopLevelStructure();

// 	CodeGraph* graph = new CodeGraph(tree);

// 	for(int i = 0; i < 100; i++) graph->step(1.0);
// 	graph->print();

// 	delete graph;
// 	delete tree;

// 	return 0;
// }

//------------------------------------------------

// int main(int argc, char* argv[])
// {
// 	std::string path1 = "test.xtm";
// 	std::string path2 = path1;
// 	if(argc > 1) 
// 	{
// 		path1 = std::string(argv[1]);
// 		path2 = path1;
// 	}
// 	if(argc > 2) path2 = std::string(argv[2]);

// 	CodeTree code1(load(path1));
// 	code1.parse();
// 	code1.printTopLevelStructure();
	
// 	CodeGraph graph(&code1);
// 	std::cout << "graph before update-----------------" << std::endl;
// 	graph.print();

// 	CodeTree code2(load(path2));
// 	code2.parse();
// 	graph.update(&code2);
// 	std::cout << "graph after update-----------------" << std::endl;
// 	graph.print();

// 	return 0;
// }


extern "C" {
	struct codegraph;

	codegraph* codegraph_create(codetree* code);
	void codegraph_destroy(codegraph* graph);

	void codegraph_update(codegraph* graph, codetree* update);
	void codegraph_step(codegraph* graph, float dt);

	void codegraph_add_child(codegraph* graph, codetree* code);

	float codegraph_get_x(codegraph* graph);
	float codegraph_get_y(codegraph* graph);
	float codegraph_get_size(codegraph* graph);

	int codegraph_get_type(codegraph* graph);
	const char* codegraph_get_tag(codegraph* graph);
	const char* codegraph_get_code(codegraph* graph);

	int codegraph_get_time_added(codegraph* graph);
	int codegraph_get_time_activated(codegraph* graph);
	int codegraph_is_active(codegraph* graph);

	int codegraph_get_child_count(codegraph* graph);
	codegraph* codegraph_get_child(codegraph* graph, int index);

	void codegraph_print(codegraph* graph);

	//void codegraph_collapse(codegraph* graph);//collapse the children of this node
}



