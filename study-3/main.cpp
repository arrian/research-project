#include "code-tree.h"

int main(int argc, char* argv[])
{
	std::cout << "Entering main" << std::endl;
	CodeTree tree(load("test.xtm"), ROOT);
	tree.printTopLevelStructure();
	//tree.print();
}