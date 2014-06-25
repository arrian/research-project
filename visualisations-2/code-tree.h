
#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <streambuf>
#include <vector>
#include <stack>
#include <locale>

/**
 * What is the state of the code from a previous version.
 **/
enum CodeDiffState
{
	ADDED,
	DELETED,
	SAME
};

/**
 * What is the current state of the code.
 **/
enum CodeEditState
{
	EDITING,
	COMPLETE,
	PARTIAL,
	INVALID
};

/**
 * What should this code be classified as.
 **/
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
	bool isActive;

	CodeForest children;

	CodeTree(std::string code, std::string tag = "root", CodeType type = ROOT);

	~CodeTree();

	void parse();

	std::size_t pushComment(std::size_t found);
	std::size_t pushBracket(std::size_t found);
	std::size_t pushString(std::size_t found);
	std::size_t pushStatement(std::size_t found);
	std::size_t pushList(std::size_t found);

	void print(int depth = 0);
	void printTopLevelStructure();

	CodeTree* getFunctionDefinition(std::string name);
	CodeForest getFunctionCalls(std::string name);
	CodeForest getDefinitions();
	CodeTree* getDsp();

	CodeTree* find(std::string argv, int argc = 0);
	CodeForest findAll(std::string argv, int argc = 0, int count = 0);

};

std::string load(std::string file);

// int main()
// {
// 	CodeTree tree(load("test.xtm"));
// 	tree.parse();
// 	tree.printTopLevelStructure();

// 	return 0;
// }

extern "C" {
	struct codetree;
	struct codeforest;

	codetree* codetree_create(char* path);
	void codetree_destroy(codetree* codetree);

	int codetree_get_children_count(codetree* codetree);
	codetree* codetree_get_child(codetree* codetree, int index);
	codeforest* codetree_get_children(codetree* codetree);
	const char* codetree_get_tag(codetree* codetree);
	int codetree_get_type(codetree* codetree);
	const char* codetree_get_code(codetree* codetree);
	bool codetree_is_active(codetree* codetree);

	codetree* codetree_find(codetree* codetree, char* name);
	//codeforest* codetree_find_all(codetree* codetree, char* name);

	void codetree_print(codetree* codetree);
	void codetree_print_top_level(codetree* codetree);
	
	int codeforest_count(codeforest* codeforest);
	codetree* codeforest_get(codeforest* codeforest, int index);
	void codeforest_print(codeforest* codeforest);

}



