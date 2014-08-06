#include "code-simple-graph.h"

#include "code-line.h"

CodeSimpleGraph::CodeSimpleGraph(std::string code)
{
	this->code = code;

	CodeSimple newCode(code);

	double y = 0.0;
	for(auto line : newCode.lines)
	{
		CodeElement element;
		element.size = (double) line.length();
		y += element.size;
		element.y = y;
		element.x = 0.0;
		element.r = 0.8;
		element.g = 0.8;
		element.b = 0.8;
		element.valid = true;
		element.userData = nullptr;

		elements.push_back(element);
	}
}

void CodeSimpleGraph::update(std::string code)
{
	CodeSimple newCode(code);

	double y = 0.0;
	for(int i = 0; i < newCode.lines.size(); i++)
	{
		if(elements.size() > i)
		{
			elements[i].size = (double) newCode.lines[i].length();
			y = elements[i].y;
			elements[i].r = 0.8;
			elements[i].g = 0.8;
			elements[i].b = 0.8;
			elements[i].valid = true;
		}
		else
		{
			CodeElement element;
			element.size = (double) newCode.lines[i].length();
			y += element.size;
			element.y = y;
			element.x = 0.0;
			element.r = 0.8;
			element.g = 0.8;
			element.b = 0.8;
			element.valid = true;
			element.userData = nullptr;

			elements.push_back(element);
		}
	}

	for(int i = newCode.lines.size(); i < elements.size(); i++)
	{
		elements[i].valid = false;
	}
}

void CodeSimpleGraph::evaluate(std::string code)
{
	for(auto& element : elements)
	{
		element.r = 0.3;
		element.g = 1.0;
		element.b = 0.3;
	}
	// CodeSimple oldCode(this.code);
	// CodeSimple newCode(code);
}

void CodeSimpleGraph::select()
{
	//spin graph here
}

void CodeSimpleGraph::error(std::string message)
{
	for(auto& element : elements)
	{
		element.r = 1.0;
		element.g = 0.3;
		element.b = 0.3;
	}
}

void CodeSimpleGraph::step(float dt)
{

}
