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

	CodeDiff getCodeDiff(long timeStart = 0, long timeEnd = std::time(0));

	std::string toString();
};


extern "C" {
	struct codehistory;

	codehistory* codehistory_create();
	void codehistory_destroy(codehistory* history);

	void codehistory_get_code_at_time(codehistory* history);
	void codehistory_get_code_current(codehistory* history);
	void codehistory_get_code_previous(codehistory* history);

	void codehistory_print(codehistory* history);

}
