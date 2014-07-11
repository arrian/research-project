#include "code-diff.h"

std::string CodeDiff::diff(std::string oldCode, std::string newCode)
{
	dtl::Diff<char, std::string> d(oldCode, newCode);
	d.compose();

	//d.printSES();

	typedef std::vector<std::pair<char, dtl::elemInfo> > SESVec;
	SESVec ses = d.getSes().getSequence();
	dtl::edit_t previousType = dtl::SES_COMMON;
	std::stringstream buffer;

	buffer << ";;same;;";
	for(SESVec::iterator it = ses.begin(); it < ses.end(); ++it)
	{
		dtl::edit_t currentType = (*it).second.type;
		if(currentType != previousType && !isspace((*it).first))//we want to ignore whitespace changes for now
		{
			if(currentType == dtl::SES_ADD) buffer << ";;add;;";
			else if(currentType == dtl::SES_DELETE) buffer << ";;delete;;";
			else if(currentType == dtl::SES_COMMON) buffer << ";;same;;";
			previousType = currentType;
		}

		buffer << (*it).first;
	}
	return buffer.str();
}