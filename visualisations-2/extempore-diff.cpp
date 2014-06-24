
#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <streambuf>
#include <vector>
#include <stack>
#include <locale>

#include "dtl/dtl.hpp"

enum CodeState
{
	ADDED,
	DELETED,
	SAME
};

enum CodeType
{
	ROOT,
	COMMENT,
	STRING,
	STATEMENT,
	FUNCTION,
	CONDITIONAL,
	LOOP,
	ERROR,
	UNKNOWN
};

class CodeTree;

typedef std::vector<CodeTree*> CodeForest;

class CodeTree
{
public:
	std::string tag;
	CodeType type;
	std::string code;
	bool isActive;//active within the interpreter
	bool isRecent;//recently added

	CodeForest children;

	CodeTree(std::string code, std::string tag = "root", CodeType type = ROOT)
	{
		this->tag = tag;
		this->type = type;
		this->code = code;
	}

	~CodeTree()
	{
		while(!children.empty()) delete children.back(), children.pop_back();
	}

	void parse()
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

	std::size_t pushComment(std::size_t found)
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

	std::size_t pushBracket(std::size_t found)
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

	std::size_t pushString(std::size_t found)
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

	std::size_t pushStatement(std::size_t found) {
		std::size_t end = code.find_first_of(" ();\n", found + 1);
		if(end - found == 0) return end;

		children.push_back(new CodeTree(code.substr(found, end - found), "statement", STATEMENT));
		return end;
	}

	std::size_t pushList(std::size_t found) {
		return found;
	}

	void print(int depth = 0)
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

	void printTopLevelStructure()
	{
		for(CodeForest::iterator it = children.begin(); it < children.end(); ++it)
		{
			std::cout << "----------------------" << std::endl << (*it)->code << std::endl;
		}
	}

	CodeForest getByArgument(std::string name, int arg = 0)
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

	CodeForest getDefinitions()
	{
		return getByArgument("define");
	}

	CodeTree* getFunctionDefinition(std::string name)
	{
		CodeForest defn = getByArgument(name, 1);

		if(defn.size() > 0) return defn[0];//just get the first (there should only be one anyway)
		return nullptr;
	}

	CodeForest getFunctionCalls(std::string name)
	{
		return getByArgument(name);
	}
};

void print(CodeTree* tree)
{
	std::cout << "-------------" << std::endl << tree->code << std::endl;
}

void print(CodeForest forest)
{
	for(CodeForest::iterator it = forest.begin(); it < forest.end(); ++it) print(*it);
}

void diff(std::string oldCode, std::string newCode)
{
	dtl::Diff<char, std::string> d(oldCode, newCode);
    d.compose();
    
    std::cout << "Shortest Edit Script" << std::endl;
    //d.printSES();

    typedef std::vector<std::pair<char, dtl::elemInfo> > SESVec;
    SESVec ses = d.getSes().getSequence();
    dtl::edit_t previousType = dtl::SES_COMMON;
    std::cout << std::endl << "---------same---------" << std::endl;
    for(SESVec::iterator it = ses.begin(); it < ses.end(); ++it)
    {
    	dtl::edit_t currentType = (*it).second.type;
    	if(currentType != previousType && !isspace((*it).first))//we want to ignore whitespace changes for now
    	{
    		std::cout << std::endl << "---------";
    		if(currentType == dtl::SES_ADD) std::cout << "add";
    		else if(currentType == dtl::SES_DELETE) std::cout << "delete";
    		else if(currentType == dtl::SES_COMMON) std::cout << "same";
    		std::cout << "---------" << std::endl;
    		previousType = currentType;
    	}

    	std::cout << (*it).first;
    }
}

void diffCodeTree(codetree* tree1, codetree* tree2)
{

}

std::string load(std::string file)
{
	std::ifstream t(file);
	std::string code((std::istreambuf_iterator<char>(t)),
		              std::istreambuf_iterator<char>());
	return code;
}

int main()
{
	std::string code1(load("test.xtm"));
	std::string code2(load("test2.xtm"));

	CodeTree tree(code1);
	tree.parse();

	tree.printTopLevelStructure();
	print(tree.getDefinitions());

	diff(code1, code2);

	return 0;
}

extern "C" {
	struct CodeTree;
	struct CodeForest;
	struct CodeDiff;

	CodeTree* codetree_create(char* path)
	{
		return new CodeTree;
	}

	void codetree_destroy(CodeTree* codetree)
	{
		delete codetree;
	}

	int codetree_get_child_function_count(CodeTree* codetree)
	{

	}
	
	CodeTree* codetree_get_child_function(CodeTree* codetree, int index);
	CodeForest* codetree_get_child_functions(CodeTree* codetree);
	char* codetree_get_tag(codetree* codetree);
	int codetree_get_type(codetree* codetree);
	char* codetree_get_code(codetree* codetree);
	bool codetree_is_active(codetree* codetree);
	void codetree_print(CodeTree* codetree);
	
	int codeforest_count(CodeForest* codeforest);
	CodeTree* codeforest_get(CodeForest* codeforest, int index);
	void codeforest_print(CodeForest* codeforest);


	//code diff comparisons
//	char* diff_add(char* previous, char* current);
//	char* diff_delete(char* previous, char* current);
//	char* diff_same(char* previous, char* current);

}



