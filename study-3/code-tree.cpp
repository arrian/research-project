#include "code-tree.h"

CodeTree::CodeTree(std::string code, CodeType type)
  : code(code),
    tokens(tokenise(code)),
    type(type)
{
	parse();
}

CodeTree::CodeTree(std::string code)
  : code(code),
    tokens(),
    type(UNKNOWN)
{
	tokens.push_back(code);
	type = getType();
}

CodeTree::CodeTree(CodeDiff* diff)
  : code(diff->newCode),
    diff(diff),
    tokens(tokenise(diff->newCode)),
    type(UNKNOWN)
{ 
	parse();
}

CodeTree::CodeTree(StringList tokens)
  : tokens(tokens),
    type(UNKNOWN)
{
	std::stringstream ss;
	for(auto token : tokens) ss << token;
	code = ss.str();

	parse();
}

CodeTree::~CodeTree()
{
	while(!children.empty()) delete children.back(), children.pop_back();
}

bool CodeTree::isSpace(std::string code)
{
	return (code.size() == 0) || (code.find_first_not_of(" \t\n\v\f\r") == std::string::npos);
}

StringList CodeTree::tokenise(std::string code)
{
    StringList tokens;
    size_t pos = 0, lastPos = 0;
    while ((pos = code.find_first_of("() \n", lastPos)) != std::string::npos)
    {
        if(lastPos != pos) tokens.push_back(code.substr(lastPos, pos - lastPos));
        tokens.push_back(code.substr(pos, 1));
        lastPos = pos + 1;
    }
    tokens.push_back(code.substr(lastPos));

    //condense comment tokens
    std::string condensed = "";
    bool found = false;
    StringList result;
    for(auto token : tokens)
    {
    	if(found == true)
    	{
    		if(token == "\n") 
    		{
    			found = false;
    		 	result.push_back(condensed);
    			condensed = "";
    		 	continue;
    		}
    		condensed += token;
    		continue;
    	}

    	if(token.size() > 0 && token[0] == ';')
    	{
    	   	found = true;
    		condensed += token;	
    	}
    	else result.push_back(token);
    }

    return result;
}

CodeType CodeTree::getType()
{
	if(type != UNKNOWN) return type;

	for(auto token : tokens)
	{
		if(token.size() == 0) continue;
		if(token == "(") return FUNCTION;
		if(token[0] == '\"') return STRING;
		if(token[0] == '\'') return LITERAL;
		if(!isSpace(token)) return STATEMENT;
	}
	return UNKNOWN;
}

/**
 * Gets the subset of elements enclosed by brackets.
 * The first element of the subset passed in should be the opening bracket.
 */
StringList CodeTree::getFunctionSubset(StringList subset)
{
	int count = 0;
	int index = 0;
	for(auto token : subset)
	{
		if(token == "(") count++;
		else if(token == ")") count--;
		index++;

		if(count == 0) return StringList(subset.begin(), subset.begin() + index); 
	}

	return StringList();
}

void CodeTree::parse()
{
	type = getType();

	if(type != FUNCTION && type != ROOT) return;

	StringList subtokens = tokens;
	if(type == FUNCTION)
	{
		bool foundFirst = true;
		for(int i = 0; i < tokens.size(); ++i)
		{
			if(isSpace(tokens[i])) continue;
			if(type == FUNCTION && tokens[i] == "(")
			{
				StringList subset = getFunctionSubset(StringList(tokens.begin() + i, tokens.end()));
				i += subset.size();
				subset.pop_back();
				subset.pop_front();
				subtokens = subset;
				break;
			}
		}
	}

	for(int index = 0; index < subtokens.size(); ++index)
	{
		std::string token = subtokens[index];

		if(isSpace(token)) continue;
		else if(token == "(") 
		{
			StringList list = getFunctionSubset(StringList(subtokens.begin() + index, subtokens.end()));
			index += list.size();//TODO: check bounds
			children.push_back(new CodeTree(list));
			continue;
		}
		else children.push_back(new CodeTree(token));
	}

}

void CodeTree::print(int depth)
{
	if(this->type == STATEMENT || this->type == STRING || this->type == LITERAL || this->type == UNKNOWN)
	{
		std::cout << tag;
		for(int i = 0; i <= depth; i++) std::cout << "----";
		std::cout << code << std::endl;
	}
	else if(this->type == FUNCTION || this->type == ROOT)
	{
		for(CodeForest::iterator it = children.begin(); it < children.end(); ++it) (*it)->print(depth + 1);
	}
}

void CodeTree::printTopLevelStructure()
{
	for(CodeForest::iterator it = children.begin(); it < children.end(); ++it)
	{
		std::cout << "----------------------" << std::endl << (*it)->code << std::endl;
	}
}

CodeTree* CodeTree::find(std::string name, int arg)
{
	for(CodeForest::iterator it = this->children.begin(); it < this->children.end(); ++it)
	{
		if((*it)->children.size() > arg && (*it)->children[arg]->code == name) return (*it);
		
		CodeTree* subNode = (*it)->find(name, arg);
		if(subNode) return subNode;
	}
	return nullptr;
}

CodeForest CodeTree::findAll(std::string name, int arg, int count)
{
	CodeForest nodes(0);
	for(CodeForest::iterator it = this->children.begin(); it < this->children.end(); ++it)
	{
		if((*it)->children.size() > arg && (*it)->children[arg]->code == name) nodes.push_back(*it);
		
		CodeForest subNodes = (*it)->findAll(name, arg);
		nodes.insert(nodes.end(), subNodes.begin(), subNodes.end());
	}
	return nodes;
}

CodeForest CodeTree::getDefinitions()
{
	return findAll("define");
}

CodeTree* CodeTree::getDsp()
{
	return find("dsp");
}

CodeTree* CodeTree::getFunctionDefinition(std::string name)
{
	CodeForest defn = findAll(name, 1);

	if(defn.size() > 0) return defn[0];//just get the first (there should only be one anyway)
	return nullptr;
}

CodeForest CodeTree::getFunctionCalls(std::string name)
{
	return findAll(name);
}

int codeTypeToInteger(CodeType type)
{
	if(type == ROOT) return 1;
	if(type == COMMENT) return 2;
	if(type == STRING) return 3;
	if(type == STATEMENT) return 4;
	if(type == FUNCTION) return 5;
	if(type == CONDITIONAL) return 6;
	if(type == LOOP) return 7;
	if(type == LITERAL) return 8;
	if(type == ERROR) return -1;
	return 0;
}

std::string load(std::string file)
{
	std::ifstream t(file);
	std::string code((std::istreambuf_iterator<char>(t)),
		              std::istreambuf_iterator<char>());
	return code;
}

codetree* codetree_create(char* path)
{
	std::cout << "Allocating codetree" << std::endl;
	std::string code = load(std::string(path));
	CodeTree* tree = new CodeTree(code, ROOT);
	tree->parse();
	return reinterpret_cast<codetree*>(tree);
}

void codetree_destroy(codetree* codetree)
{
	std::cout << "Deallocating codetree" << std::endl;
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

	return codeTypeToInteger(type);
}

const char* codetree_get_code(codetree* codetree)
{
	return reinterpret_cast<CodeTree*>(codetree)->code.c_str();
}

bool codetree_is_active(codetree* codetree)
{
	return reinterpret_cast<CodeTree*>(codetree)->isActive;
}

codetree* codetree_find(codetree* codetree, char* name)
{
	return reinterpret_cast<struct codetree*>(reinterpret_cast<CodeTree*>(codetree)->find(name));
}

// codeforest* codetree_find_all(codetree* codetree, char* name)
// {
// 	return reinterpret_cast<codeforest*>(&(reinterpret_cast<CodeTree*>(codetree)->findAll(name)));
// }

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




