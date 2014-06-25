
#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <streambuf>
#include <vector>
#include <stack>
#include <locale>

#include "code-tree.h"

class ForceGraph
{
public:
	ForceGraph(CodeTree* code);
	ForceGraph(CodeTree* code, ForceGraph* parent);
	~ForceGraph();

	void update(CodeTree* code);
	void step(float dt);

	float x;
	float y;
	float size;

	ForceGraph* parent;
	std::vector<ForceGraph*> children;

	CodeTree* data;

	void print(int depth = 0);
};

int main()
{
	CodeTree* tree = new CodeTree(load("test.xtm"));
	tree->parse();
	tree->printTopLevelStructure();

	ForceGraph* graph = new ForceGraph(tree);
	graph->step(1.0);
	graph->print();

	delete graph;
	delete tree;

	return 0;
}

extern "C" {
	struct forcegraph;

	forcegraph* forcegraph_create(codetree* code);
	void forcegraph_destroy(forcegraph* graph);

	void forcegraph_update(forcegraph* graph, codetree* update);
	void forcegraph_step(forcegraph* graph, float dt);

	float forcegraph_get_x(forcegraph* graph);
	float forcegraph_get_y(forcegraph* graph);
	float forcegraph_get_size(forcegraph* graph);

	int forcegraph_get_child_count(forcegraph* graph);
	forcegraph* forcegraph_get_child(forcegraph* graph, int index);
	codetree* forcegraph_get_code(forcegraph* graph);

	void forcegraph_print(forcegraph* graph);

	//void forcegraph_collapse(forcegraph* graph);//collapse the children of this node
}



