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
			std::cout << "comparing " << graphs[j]->code << std::endl << " with " << tree.children[i]->code << std::endl;
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

	// for(auto graph : this->graphs)
	// {
	// 	graph->select();
	// }
	int counter = 0;
	for(int i = 0; i < tree.children.size(); i++)
	{
		int current = tree.children[i]->code.length();
		if(selection >= counter && selection <= counter + current)
		{
			CodeSimpleGraph* graph = findGraph(tree.children[i]->code);
			if(graph) graph->select();
		}
		counter += current;
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




//////////////////////////////////////

code_manager* code_manager_create(char* code)
{
	return reinterpret_cast<code_manager*>(new CodeManager(code));
}

void code_manager_update(code_manager* manager, char* code)
{
	reinterpret_cast<CodeManager*>(manager)->update(code);
}

void code_manager_evaluate(code_manager* manager, char* code)
{
	reinterpret_cast<CodeManager*>(manager)->evaluate(code);
}

void code_manager_select(code_manager* manager, int selection)
{
	reinterpret_cast<CodeManager*>(manager)->select(selection);
}

void code_manager_error(code_manager* manager, char* message)
{
	reinterpret_cast<CodeManager*>(manager)->error(message);
}

void code_manager_step(code_manager* manager, float dt)
{
	reinterpret_cast<CodeManager*>(manager)->step(dt);
}

int code_manager_get_graph_count(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->graphs.size();
}

code_graph* code_manager_get_graph(code_manager* manager, int index)
{
	CodeManager* codeManager = reinterpret_cast<CodeManager*>(manager);
	return reinterpret_cast<code_graph*>(codeManager->graphs[index]);
}

int code_graph_get_element_count(code_graph* graph)
{
	return reinterpret_cast<CodeSimpleGraph*>(graph)->elements.size();
}

code_element* code_graph_get_element(code_graph* graph, int index)
{
	CodeSimpleGraph* codeGraph = reinterpret_cast<CodeSimpleGraph*>(graph);
	return reinterpret_cast<code_element*>(&codeGraph->elements[index]);
}

void* code_graph_get_widget(code_graph* graph)
{
	// std::cout << "getting graph user data" << reinterpret_cast<CodeSimpleGraph*>(graph)->widget << std::endl;
	return reinterpret_cast<CodeSimpleGraph*>(graph)->widget;
}

void code_graph_set_widget(code_graph* graph, void* widget)
{
	// std::cout << "setting graph user data" << widget << std::endl;
	reinterpret_cast<CodeSimpleGraph*>(graph)->widget = widget;
}

void* code_graph_get_entity(code_graph* graph)
{
	return reinterpret_cast<CodeSimpleGraph*>(graph)->entity;
}

void code_graph_set_entity(code_graph* graph, void* entity)
{
	reinterpret_cast<CodeSimpleGraph*>(graph)->entity = entity;
}

double code_element_get_size(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->size;
}

double code_element_get_x(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->x;
}

double code_element_get_y(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->y;
}

double code_element_get_r(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->r;
}

double code_element_get_g(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->g;
}

double code_element_get_b(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->b;
}

bool code_element_is_valid(code_element* element)
{
	return reinterpret_cast<CodeElement*>(element)->valid;
}

void* code_element_get_user_data(code_element* element)
{
	std::cout << "getting element user data" << reinterpret_cast<CodeElement*>(element)->userData << std::endl;
	return reinterpret_cast<CodeElement*>(element)->userData;
}

void code_element_set_user_data(code_element* element, void* userData)
{
	std::cout << "setting element user data" << userData << std::endl;
	reinterpret_cast<CodeElement*>(element)->userData = userData;
}





