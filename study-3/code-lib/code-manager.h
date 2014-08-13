#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "code-line.h"
#include "code-simple-graph.h"

//#include "code-graph.h"


class CodeManager
{
public:
	CodeManager(std::string code);
	~CodeManager();

	void update(std::string code);//updates the offline code
	void evaluate(std::string code);//updates the online code
	void select(int selection);//simple selection
	void error(std::string message);//error from evaluation

	void step(float dt);

	CodeSimpleGraph* findGraph(std::string code);

	std::string code;//most recent code
	std::string evaluation;//most recent evaluation... most likely to cause error
	std::vector<CodeSimpleGraph*> graphs;//graph for each function

};

extern "C" 
{
	struct code_manager;
	struct code_graph;
	struct code_element;

	code_manager* code_manager_create(char* code);
	void code_manager_update(code_manager* manager, char* code);
	void code_manager_evaluate(code_manager* manager, char* code);
	void code_manager_select(code_manager* manager, int selection);
	void code_manager_error(code_manager* manager, char* message);
	void code_manager_step(code_manager* manager, float dt);
	int code_manager_get_graph_count(code_manager* manager);
	code_graph* code_manager_get_graph(code_manager* manager, int index);

	int code_graph_get_element_count(code_graph* graph);
	code_element* code_graph_get_element(code_graph* graph, int index);
	void* code_graph_get_widget(code_graph* graph);
	void code_graph_set_widget(code_graph* graph, void* widget);
	void* code_graph_get_entity(code_graph* graph);
	void code_graph_set_entity(code_graph* graph, void* entity);

	// double code_element_get_size(code_element* element);
	// double code_element_get_x(code_element* element);
	// double code_element_get_y(code_element* element);
	// double code_element_get_r(code_element* element);
	// double code_element_get_g(code_element* element);
	// double code_element_get_b(code_element* element);
	bool code_element_is_valid(code_element* element);
	void* code_element_get_user_data(code_element* element);
	void code_element_set_user_data(code_element* element, void* userData);
}

// void printX(code_manager* manager)
// {
// 	for(int i = 0; i < code_manager_get_graph_count(manager); i++)
// 	{
// 		code_graph* graph = code_manager_get_graph(manager, i);
// 		for(int j = 0; j < code_graph_get_element_count(graph); j++)
// 		{
// 			code_element* element = code_graph_get_element(graph, j);
// 			std::cout << "found y of: " << code_element_get_y(element) << std::endl;
// 		}
// 	}
// }

int main(int argc, char* argv[])
{	
	std::string code1 = "(test function\n here)\n\n(another3 test \nfunction)\n";
	CodeManager manager(code1);
	std::string code2 = "(test function\n here2)\n\n(another test \nfunction2)\n(another2 test \nfunction2)\n";
	manager.update(code2);

	// printX(reinterpret_cast<code_manager*>(&manager));

	std::cout << manager.graphs.size() << std::endl;

}





