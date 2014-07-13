#include "code-history.h"

void CodeHistory::addCode(std::string code, long time)
{
	codeCache.push_back(std::pair<long,std::string>(time, code));
}

void CodeHistory::addSelection(std::string selection, long time)
{
	selectionCache.push_back(std::pair<long,std::string>(time, selection));	
}

void CodeHistory::addEvaluation(std::string evaluation, long time)
{
	evaluationCache.push_back(std::pair<long,std::string>(time, evaluation));
}

void CodeHistory::addError(std::string error, long time)
{
	errorCache.push_back(std::pair<long,std::string>(time, error));
}

std::string CodeHistory::getCodeAtTime(long time)
{
	std::string previous = "";
	long previousTime = -1;
	for(auto item : codeCache)
	{
		if(previousTime == -1)
		{
			previousTime = item.first;
			previous = item.second;
			continue;
		}

		if(item.first > time) break;
		previous = item.second;
	}

	return previous;
}

std::string CodeHistory::getCodeCurrent()
{
	if(codeCache.empty()) return "";
	return codeCache.back().second;
}

std::string CodeHistory::getCodePrevious()
{
	if(codeCache.size() < 2) return "";
	return codeCache.end()[-2].second;
}

CodeDiff CodeHistory::getCodeDiff(long timeStart, long timeEnd)
{
	std::string start = getCodeAtTime(timeStart);
	std::string end = getCodeAtTime(timeEnd);

	return CodeDiff(start, end);
}

std::string CodeHistory::toString()
{
	std::stringstream buffer;
	buffer << "Code Cache: " << codeCache.size() << std::endl;
	buffer << "Selection Cache: " << selectionCache.size() << std::endl;
	buffer << "Evaluation Cache: " << evaluationCache.size() << std::endl;
	buffer << "Error Cache: " << errorCache.size() << std::endl;
	return buffer.str();
}



