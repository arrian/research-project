
#pragma once

#include <iostream>
#include <string>
#include <cstddef>
#include <fstream>
#include <streambuf>
#include <vector>
#include <stack>
#include <locale>

#include "code-diff.h"

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
	LITERAL,
	CONDITIONAL,
	LOOP,
	EMPTY,
	ERROR,
	UNKNOWN
};

int codeTypeToInteger(CodeType type);

class CodeTree;

typedef std::vector<CodeTree*> CodeForest;
typedef std::deque<std::string> StringList;

class CodeTree
{
public:
	std::string tag;
	CodeType type;
	std::string code;
	bool isActive;
	CodeDiff* diff;
	StringList tokens;

	CodeForest children;

	CodeTree(std::string code, CodeType type);//construct using ROOT
	CodeTree(CodeDiff* code);

	~CodeTree();

	StringList tokenise(std::string code);
	bool isSpace(std::string code);
	CodeType getType();
	StringList getFunctionSubset(StringList subset);

	void parse();

	void print(int depth = 0);
	void printTopLevelStructure();

	CodeTree* getFunctionDefinition(std::string name);
	CodeForest getFunctionCalls(std::string name);
	CodeForest getDefinitions();
	CodeTree* getDsp();

	CodeTree* find(std::string argv, int argc = 0);
	CodeForest findAll(std::string argv, int argc = 0, int count = 0);

private:
	int getBracketClose(std::string code, int bracketOpen);
	int getCommentClose(std::string code, int commentOpen);

	CodeTree(std::string code);
	CodeTree(StringList tokens);

};

std::string load(std::string file);

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



