
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

int main()
{
	CodeTree code(load("test.xtm"));
	code.parse();
	code.printTopLevelStructure();
	
	ForceGraph graph(&code);
	graph.print();
	graph.update(&code);
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

	int forcegraph_get_child_count(forcegraph* graph);
	forcegraph* forcegraph_get_child(forcegraph* graph, int index);
	codetree* forcegraph_get_code(forcegraph* graph);

	void forcegraph_print(forcegraph* graph);

	//void forcegraph_collapse(forcegraph* graph);//collapse the children of this node
}



