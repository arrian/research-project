#include "code-manager.h"

#include "code-tree.h"//for initial top level splitting

CodeManager::CodeManager(std::string code)
{
	this->code = code;
	CodeTree tree(code, ROOT);

	for(int i = 0; i < tree.children.size(); i++)
	{
		graphs.push_back(new CodeSimpleGraph(tree.children[i]->code));
	}
}

CodeManager::~CodeManager()
{
	for(int i = 0; i < graphs.size(); i++)
	{
		delete graphs[i];
	}
	graphs.clear();
}

CodeSimpleGraph* CodeManager::findGraph(std::string code)
{
	for(int j = 0; j < graphs.size(); j++)
	{
		if(CodeSimple(graphs[j]->code).isFirstLineMatch(code))
		{
			// std::cout << "found first line match - " << code << " - " << graphs[j]->code << std::endl;
			return graphs[j];
		}
	}
	return nullptr;
}

void CodeManager::update(std::string code)
{
	bool found = false;
	CodeTree tree(code, ROOT);

	std::vector<CodeSimpleGraph*> unfoundGraphs;

	for(int j = 0; j < graphs.size(); j++)
	{
		unfoundGraphs.push_back(graphs[j]);
	}

	for(int i = 0; i < tree.children.size(); i++)
	{
		found = false;
		CodeSimpleGraph* graph = findGraph(tree.children[i]->code);
		if(graph)
		{
			graph->update(tree.children[i]->code);
			unfoundGraphs.erase(std::remove(unfoundGraphs.begin(), unfoundGraphs.end(), graph), unfoundGraphs.end());
			continue;
		}
		graphs.push_back(new CodeSimpleGraph(tree.children[i]->code));
	}

	for(int j = 0; j < unfoundGraphs.size(); j++)
	{
		// std::cout << "removing " << unfoundGraphs[j]->code << std::endl;
		graphs.erase(std::remove(graphs.begin(), graphs.end(), unfoundGraphs[j]), graphs.end());
	}
}

void CodeManager::evaluate(std::string code)
{
	CodeTree tree(code, ROOT);

	for(int i = 0; i < tree.children.size(); i++)
	{
		for(int j = 0; j < graphs.size(); j++)
		{
			if(CodeSimple(graphs[j]->code).isFirstLineMatch(tree.children[i]->code))
			{
				graphs[j]->evaluate(tree.children[i]->code);
			}
		}
	}
}

void CodeManager::select(int selection)
{
	//temp hack
	CodeTree tree(this->code, ROOT);

	int counter = 0;
	for(int i = 0; i < tree.children.size(); i++)
	{
		int current = tree.children[i]->code.length();
		if(selection >= counter && selection <= counter + current)
		{
			CodeSimpleGraph* graph = findGraph(tree.children[i]->code);
			if(graph) graph->select();
		}		
	}
}

void CodeManager::error(std::string message)
{
	CodeTree tree(this->evaluation, ROOT);

	for(int i = 0; i < tree.children.size(); i++)
	{
		for(int j = 0; j < graphs.size(); j++)
		{
			if(CodeSimple(graphs[j]->code).isFirstLineMatch(tree.children[i]->code))
			{
				graphs[j]->error(message);
			}
		}
	}
}

void CodeManager::step(float dt)
{
	for(int j = 0; j < graphs.size(); j++)
	{
		graphs[j]->step(dt);
	}
}

