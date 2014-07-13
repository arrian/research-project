#include "code-diff.h"

CodeDiff::CodeDiff(std::string oldCode, std::string newCode)
  : oldCode(oldCode),
    newCode(newCode),
    diffCode(diff(oldCode, newCode))
{
}

CodeDiff::CodeDiff(std::vector<CodeDiffFragment> diffCode)
  : diffCode(diffCode),
  	oldCode(""),
  	newCode("")
{
	for(auto &diff : diffCode)
	{
		//change to stringstream
		if(diff.state == ADD || diff.state == SAME) newCode += diff.fragment;
		if(diff.state == DELETE || diff.state == SAME) oldCode += diff.fragment;
	}
}

CodeDiff CodeDiff::CodeDiffSubstring(int start, int end)
{
	std::vector<CodeDiffFragment> newDiffCode;
	int index = 0;
	bool foundStart = false;
	for(auto &diff : diffCode)
	{
		if(foundStart)
		{
			CodeDiffFragment newDiff;
			newDiff.fragment = diff.fragment;
			newDiff.state = diff.state;

			if(index + newDiff.fragment.size() >= end)
			{
				newDiff.fragment = newDiff.fragment.substr(0, end - index);
				if(newDiff.state == ADD || newDiff.state == SAME) index += newDiff.fragment.size();
				newDiffCode.push_back(newDiff);
				break;
			}
			if(newDiff.state == ADD || newDiff.state == SAME) index += newDiff.fragment.size();
			newDiffCode.push_back(newDiff);
			continue;
		}

		if(index + diff.fragment.size() > start)
		{
			foundStart = true;
			CodeDiffFragment newDiff;
			newDiff.fragment = diff.fragment.substr(start - index, diff.fragment.size() - (start - index));
			newDiff.state = diff.state;
			if(newDiff.state == ADD || newDiff.state == SAME) index += newDiff.fragment.size();
			newDiffCode.push_back(newDiff);
		}
	}

	return CodeDiff(newDiffCode);
}

std::vector<CodeDiffFragment> CodeDiff::diff(std::string oldCode, std::string newCode)
{
	std::vector<CodeDiffFragment> newDiffCode;

	dtl::Diff<char, std::string> d(oldCode, newCode);
	d.compose();

	//d.printSES();

	typedef std::vector<std::pair<char, dtl::elemInfo> > SESVec;
	SESVec ses = d.getSes().getSequence();
	dtl::edit_t previousType = dtl::SES_COMMON;
	std::stringstream buffer;

	CodeDiffState state = SAME;
	dtl::edit_t currentType = dtl::SES_COMMON;
	for(SESVec::iterator it = ses.begin(); it < ses.end(); ++it)
	{
		currentType = (*it).second.type;
		if(currentType != previousType && !isspace((*it).first))//we want to ignore whitespace changes for now
		{
			newDiffCode.push_back(fragmentFromDtl(previousType, buffer.str()));
			buffer.str("");

			previousType = currentType;
		}

		buffer << (*it).first;
	}
	newDiffCode.push_back(fragmentFromDtl(currentType, buffer.str()));

	return newDiffCode;
}

CodeDiffFragment CodeDiff::fragmentFromDtl(dtl::edit_t type, std::string fragment)
{
	CodeDiffFragment newDiff;
	newDiff.fragment = fragment;
	if(type == dtl::SES_ADD) newDiff.state = ADD;
	else if(type == dtl::SES_DELETE) newDiff.state = DELETE;
	else if(type == dtl::SES_COMMON) newDiff.state = SAME;
	return newDiff;
}


