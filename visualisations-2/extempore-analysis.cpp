#include "extempore-analysis.h"

CodeTree::CodeTree(std::string code, std::string tag, CodeType type)
{
	this->tag = tag;
	this->type = type;
	this->code = code;
}

CodeTree::~CodeTree()
{
	while(!children.empty()) delete children.back(), children.pop_back();
}

void CodeTree::parse()
{
	if(code.empty()) return;

	for(int index = 0; index < code.length(); index++)
	{
		if(code[index] == ';') index = pushComment(index);
		else if(code[index] == '(') index = pushBracket(index);
		else if(code[index] == '"') index = pushString(index);
		else if(isspace(code[index])) index++;
		else index = pushStatement(index);

		if(index == -1) break;
	}
}

std::size_t CodeTree::pushComment(std::size_t found)
{
	std::size_t end;
	for(end = found; end < code.length(); end++)
	{
		if(code[end] == '\n')
		{
			if(end + 1 < code.length() && code[end + 1] == ';') end++;
			else break;
		}
	}

	children.push_back(new CodeTree(code.substr(found, end - found), "comment", COMMENT));
	return end;
}

std::size_t CodeTree::pushBracket(std::size_t found)
{
	std::size_t end;
	int depth = 0;
	for(end = found; end < code.length(); end++)
	{	
		if(code[end] == '(') depth++;
		else if(code[end] == ')') depth--;

		if(depth <= 0) break;
	}

	if(end == std::string::npos || end - found == 0) return end;

	//lots of optimisation possible here
	CodeTree* subtree = new CodeTree(code.substr(found + 1, end - (found + 1)), "function", FUNCTION);
	subtree->parse();
	children.push_back(subtree);
	return end;
}

std::size_t CodeTree::pushString(std::size_t found)
{
	int i;
	for(i = found + 1; i < code.length(); i++)
	{
		if('\\' == code[found]) i++;//skip escaped strings
		else if('\"' == code[found]) break;
	}
	children.push_back(new CodeTree(code.substr(found, i - found), "string", STRING));
	return i;
}

std::size_t CodeTree::pushStatement(std::size_t found) {
	std::size_t end = code.find_first_of(" ();\n", found + 1);
	if(end - found == 0) return end;

	children.push_back(new CodeTree(code.substr(found, end - found), "statement", STATEMENT));
	return end;
}

std::size_t CodeTree::pushList(std::size_t found) {
	return found;
}

void CodeTree::print(int depth)
{
	if(this->type == STATEMENT || this->type == STRING)
	{
		std::cout << tag;
		for(int i = 0; i <= depth; i++) std::cout << "----";
		std::cout << code << std::endl;
	}
	else if(this->type == FUNCTION || this->type == ROOT)
	{
		int i = 0;
		for(CodeForest::iterator it = children.begin(); it < children.end(); ++it)
		{
			if(i == 0)
			{
				i++;
				std::cout << tag;
					(*it)->print(depth + 1);
				
				continue;
			}
			(*it)->print(depth + 1);
			i++;
		}
	}
}

void CodeTree::printTopLevelStructure()
{
	for(CodeForest::iterator it = children.begin(); it < children.end(); ++it)
	{
		std::cout << "----------------------" << std::endl << (*it)->code << std::endl;
	}
}

CodeForest CodeTree::getByArgument(std::string name, int arg)
{
	CodeForest nodes(0);
	for(CodeForest::iterator it = this->children.begin(); it < this->children.end(); ++it)
	{
		if((*it)->children.size() > arg && (*it)->children[arg]->code == name) nodes.push_back(*it);
		
		CodeForest subNodes = (*it)->getByArgument(name, arg);
		nodes.insert(nodes.end(), subNodes.begin(), subNodes.end());
	}
	return nodes;
}

CodeForest CodeTree::getDefinitions()
{
	return getByArgument("define");
}

CodeTree* CodeTree::getFunctionDefinition(std::string name)
{
	CodeForest defn = getByArgument(name, 1);

	if(defn.size() > 0) return defn[0];//just get the first (there should only be one anyway)
	return nullptr;
}

CodeForest CodeTree::getFunctionCalls(std::string name)
{
	return getByArgument(name);
}

codetree* codetree_create(char* path)
{
	std::string code = load(std::string(path));
	CodeTree* tree = new CodeTree(code);
	tree->parse();
	return reinterpret_cast<codetree*>(tree);
}

void codetree_destroy(codetree* codetree)
{
	delete reinterpret_cast<CodeTree*>(codetree);
}


int codetree_get_children_count(codetree* codetree)
{
	return reinterpret_cast<CodeTree*>(codetree)->children.size();
}

codetree* codetree_get_child(codetree* codetree, int index)
{
	return reinterpret_cast<struct codetree*>(reinterpret_cast<CodeTree*>(codetree)->children[index]);
}

codeforest* codetree_get_children(codetree* codetree)
{
	return reinterpret_cast<codeforest*>(&(reinterpret_cast<CodeTree*>(codetree)->children));
}

const char* codetree_get_tag(codetree* codetree)
{
	return reinterpret_cast<CodeTree*>(codetree)->tag.c_str();
}

int codetree_get_type(codetree* codetree)
{
	CodeType type = reinterpret_cast<CodeTree*>(codetree)->type;

	if(type == ROOT) return 1;
	return 2;
}

const char* codetree_get_code(codetree* codetree)
{
	return reinterpret_cast<CodeTree*>(codetree)->code.c_str();
}

bool codetree_is_active(codetree* codetree)
{
	return reinterpret_cast<CodeTree*>(codetree)->isActive;
}

void codetree_print(codetree* codetree)
{
	std::cout << "-------------" << std::endl << reinterpret_cast<CodeTree*>(codetree)->code << std::endl;
}

void codetree_print_top_level(codetree* codetree)
{
	reinterpret_cast<CodeTree*>(codetree)->printTopLevelStructure();	
}

int codeforest_count(codeforest* codeforest)
{
	return reinterpret_cast<CodeForest*>(codeforest)->size();	
}

codetree* codeforest_get(codeforest* codeforest, int index)
{
	return reinterpret_cast<codetree*>(reinterpret_cast<CodeForest*>(codeforest)->at(index));
}

void codeforest_print(codeforest* codeforest)
{
	for(CodeForest::iterator it = reinterpret_cast<CodeForest*>(codeforest)->begin(); it < reinterpret_cast<CodeForest*>(codeforest)->end(); ++it) (*it)->print();
}




