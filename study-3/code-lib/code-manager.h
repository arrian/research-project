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







}


int main(int argc, char* argv[])
{	
	std::string code1 = "(test function\n here)\n\n(another3 test \nfunction)\n";
	CodeManager manager(code1);
	std::string code2 = "(test function\n here2)\n\n(another test \nfunction2)\n(another2 test \nfunction2)\n";
	manager.update(code2);

	std::cout << manager.graphs.size() << std::endl;

}











