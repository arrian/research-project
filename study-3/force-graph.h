
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

class ForceGraph
{
public:
	ForceGraph(CodeTree* code, ForceGraph* parent = nullptr);
	~ForceGraph();

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

	ForceGraph* parent;
	std::vector<ForceGraph*> children;

	std::string tag;
	CodeType type;
	std::string code;
	//CodeTree* data;

	void print(int depth = 0);

	std::vector<ForceGraph*> getVertices();
	std::vector<std::pair<ForceGraph*, ForceGraph*> > getEdges();
};

// int main()
// {
// 	CodeTree* tree = new CodeTree(load("test.xtm"));
// 	tree->parse();
// 	tree->printTopLevelStructure();

// 	ForceGraph* graph = new ForceGraph(tree);

// 	for(int i = 0; i < 100; i++) graph->step(1.0);
// 	graph->print();

// 	delete graph;
// 	delete tree;

// 	return 0;
// }

int main(int argc, char* argv[])
{
	std::string path1 = "test.xtm";
	std::string path2 = path1;
	if(argc > 1) 
	{
		path1 = std::string(argv[1]);
		path2 = path1;
	}
	if(argc > 2) path2 = std::string(argv[2]);

	CodeTree code1(load(path1));
	code1.parse();
	code1.printTopLevelStructure();
	
	ForceGraph graph(&code1);
	std::cout << "graph before update-----------------" << std::endl;
	graph.print();

	CodeTree code2(load(path2));
	code2.parse();
	graph.update(&code2);
	std::cout << "graph after update-----------------" << std::endl;
	graph.print();

	return 0;
}


extern "C" {
	struct forcegraph;

	forcegraph* forcegraph_create(codetree* code);
	void forcegraph_destroy(forcegraph* graph);

	void forcegraph_update(forcegraph* graph, codetree* update);
	void forcegraph_step(forcegraph* graph, float dt);

	void forcegraph_add_child(forcegraph* graph, codetree* code);

	float forcegraph_get_x(forcegraph* graph);
	float forcegraph_get_y(forcegraph* graph);
	float forcegraph_get_size(forcegraph* graph);

	int forcegraph_get_type(forcegraph* graph);
	const char* forcegraph_get_tag(forcegraph* graph);
	const char* forcegraph_get_code(forcegraph* graph);

	int forcegraph_get_time_added(forcegraph* graph);
	int forcegraph_get_time_activated(forcegraph* graph);
	int forcegraph_is_active(forcegraph* graph);

	int forcegraph_get_child_count(forcegraph* graph);
	forcegraph* forcegraph_get_child(forcegraph* graph, int index);

	void forcegraph_print(forcegraph* graph);

	//void forcegraph_collapse(forcegraph* graph);//collapse the children of this node
}



