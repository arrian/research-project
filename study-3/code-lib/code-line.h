#pragma once

#include <string>
#include <sstream>
#include <vector>

class CodeSimple
{
public:
	CodeSimple(std::string code);

	std::string getCode();
	int getLineCount();
	std::string getLineAt(int index);

	bool isFirstLineMatch(std::string other);
	bool isFirstLineVoid();//eg: '(define drums)'

	std::vector<std::string> lines;

private:

	std::vector<std::string> split(std::string s, std::string delim);
};