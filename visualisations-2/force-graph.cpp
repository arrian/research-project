#include "force-graph.h"


ForceGraph::ForceGraph(CodeTree* code)
	: x(0.0),
	  y(0.0),
	  size(10.0),
	  data(code),
	  parent(nullptr),
	  children()
{
	//iterate children of code
}

ForceGraph::ForceGraph(CodeTree* code, ForceGraph* parent)
	: x(0.0),
	  y(0.0),
	  size(10.0),
	  data(code),
	  parent(parent),
	  children()
{
	//iterate children of code
}

ForceGraph::~ForceGraph()
{
	while(!children.empty()) delete children.back(), children.pop_back();
}

void ForceGraph::update(CodeTree* code)
{
	//iterate this and code and compare
}

void ForceGraph::step(float dt)
{
	//iterate this and call step
	//update location based on parents and children
}

void ForceGraph::print(int depth)
{
	//iterate this printing all children
}






forcegraph* forcegraph_create(codetree* code)
{
	return reinterpret_cast<forcegraph*>(new ForceGraph(reinterpret_cast<CodeTree*>(code)));
}

void forcegraph_destroy(forcegraph* graph)
{
	delete reinterpret_cast<ForceGraph*>(graph);
}

void forcegraph_update(forcegraph* graph, codetree* update)
{
	(reinterpret_cast<ForceGraph*>(graph))->update(reinterpret_cast<CodeTree*>(update));
}

void forcegraph_step(forcegraph* graph, float dt)
{
	(reinterpret_cast<ForceGraph*>(graph))->step(dt);
}

float forcegraph_get_x(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->x;
}

float forcegraph_get_y(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->y;
}

float forcegraph_get_size(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->size;
}

int forcegraph_get_child_count(forcegraph* graph)
{
	return (reinterpret_cast<ForceGraph*>(graph))->children.size();
}

forcegraph* forcegraph_get_child(forcegraph* graph, int index)
{
	return reinterpret_cast<forcegraph*>((reinterpret_cast<ForceGraph*>(graph))->children.at(index));
}

codetree* forcegraph_get_code(forcegraph* graph)
{
	return (reinterpret_cast<codetree*>(reinterpret_cast<ForceGraph*>(graph)->data));
}

void forcegraph_print(forcegraph* graph)
{
	(reinterpret_cast<ForceGraph*>(graph))->print();	
}



