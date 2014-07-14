#include "code-tree.h"

int main(int argc, char* argv[])
{
	std::cout << "Entering main" << std::endl;
	CodeTree tree(load("test.xtm"), ROOT);
	tree.printTopLevelStructure();
	//tree.print();
}

// int main(int argc, char* argv[])
// {
// 	CodeHistory codeHistory;

// 	codeHistory.addCode("(test text)", 0);
// 	codeHistory.addSelection("1,1,1,4:1,4");//selections:cursor positions
// 	codeHistory.addEvaluation("test");
// 	codeHistory.addError("failed to evaluate test");
// 	codeHistory.addCode("(test text) (more stuff here)");
// 	codeHistory.addSelection("1,1,1,4:1,4");
// 	codeHistory.addEvaluation("(test text)");

// 	std::cout << "Current Time: " << std::time(0) << std::endl;
// 	std::cout << "Current Code: " << codeHistory.getCodeCurrent() << std::endl;
// 	std::cout << "Previous Code: " << codeHistory.getCodePrevious() << std::endl;
// 	std::cout << "At Time 0 Code: " << codeHistory.getCodeAtTime(0) << std::endl;

// 	std::cout << "Code History State: " << std::endl << codeHistory.toString() << std::endl;
// 	std::cout << "Code History Example: " << codeHistory.getCodeDiff(0, std::time(0)).newCode << std::endl;

// 	return 0;
// }

// int main()
// {
// 	CodeTree* tree = new CodeTree(load("test.xtm"));
// 	tree->parse();
// 	tree->printTopLevelStructure();

// 	CodeGraph* graph = new CodeGraph(tree);

// 	for(int i = 0; i < 100; i++) graph->step(1.0);
// 	graph->print();

// 	delete graph;
// 	delete tree;

// 	return 0;
// }

//------------------------------------------------

// int main(int argc, char* argv[])
// {
// 	std::string path1 = "test.xtm";
// 	std::string path2 = path1;
// 	if(argc > 1) 
// 	{
// 		path1 = std::string(argv[1]);
// 		path2 = path1;
// 	}
// 	if(argc > 2) path2 = std::string(argv[2]);

// 	CodeTree code1(load(path1));
// 	code1.parse();
// 	code1.printTopLevelStructure();
	
// 	CodeGraph graph(&code1);
// 	std::cout << "graph before update-----------------" << std::endl;
// 	graph.print();

// 	CodeTree code2(load(path2));
// 	code2.parse();
// 	graph.update(&code2);
// 	std::cout << "graph after update-----------------" << std::endl;
// 	graph.print();

// 	return 0;
// }