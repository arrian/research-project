#pragma once

#include <time.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "code-diff.h"

class CodeHistory
{
public:
	long startTime;
	long lastTime;

	std::vector<std::pair<long, std::string> > codeCache;
	std::vector<std::pair<long, std::string> > selectionCache;
	std::vector<std::pair<long, std::string> > evaluationCache;
	std::vector<std::pair<long, std::string> > errorCache;

	void addCode(std::string code, long time = std::time(0));
	void addSelection(std::string selection, long time = std::time(0));
	void addEvaluation(std::string evaluation, long time = std::time(0));
	void addError(std::string error, long time = std::time(0));

	std::string getCodeAtTime(long time);
	std::string getCodeCurrent();
	std::string getCodePrevious();

	std::string getCodeDiff(long timeStart = 0, long timeEnd = std::time(0));

	std::string toString();
};

int main(int argc, char* argv[])
{
	CodeHistory codeHistory;

	codeHistory.addCode("(test text)", 0);
	codeHistory.addSelection("1,1,1,4:1,4");//selections:cursor positions
	codeHistory.addEvaluation("test");
	codeHistory.addError("failed to evaluate test");
	codeHistory.addCode("(test text) (more stuff here)");
	codeHistory.addSelection("1,1,1,4:1,4");
	codeHistory.addEvaluation("(test text)");

	std::cout << "Current Time: " << std::time(0) << std::endl;
	std::cout << "Current Code: " << codeHistory.getCodeCurrent() << std::endl;
	std::cout << "Previous Code: " << codeHistory.getCodePrevious() << std::endl;
	std::cout << "At Time 0 Code: " << codeHistory.getCodeAtTime(0) << std::endl;

	std::cout << "Code History State: " << std::endl << codeHistory.toString() << std::endl;
	std::cout << "Code History Example: " << codeHistory.getCodeDiff(0, std::time(0)) << std::endl;

	return 0;
}
