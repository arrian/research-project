
#pragma once
#pragma once

#include <vector>
#include <iostream>
#include <string>


struct CodeElement
{
	double size;
	double x;
	double y;

	double r;
	double g;
	double b;

	bool valid;//is this graph ready for destruction or is it valid?

	void* userData;
};

class CodeSimpleGraph
{
public:
	std::string code;
	std::vector<CodeElement> elements;

	void* userData;
	
	CodeSimpleGraph(std::string code);

	void update(std::string code);//updates the offline code
	void evaluate(std::string code);//updates the online code
	void select();//simple selection
	void error(std::string message);//error from evaluation

	void step(float dt);

// private:
// 	void addElement(std::string line);
};