
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
	
	bool isShallowMatch(CodeTree* code);
	bool isDeepMatch(CodeTree* code);

	//bool isSubset(CodeTree* );
	//bool isSuperset(CodeTree* );

	//frame size
	float width;
	float height;

	float x;
	float y;
	float size;
	float vx;
	float vy;

	float tx;//temp location
	float ty;//temp location

	float temperature;
	void cool();

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
	void codegraph_set_active(codegraph* graph, char* defname);

	int codegraph_get_child_count(codegraph* graph);
	codegraph* codegraph_get_child(codegraph* graph, int index);

	void codegraph_print(codegraph* graph);
}



