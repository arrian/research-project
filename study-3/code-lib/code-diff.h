#pragma once

#include <sstream>
#include "deps/dtl/dtl.hpp"

/**
 * Defines the state of the code from a previous version.
 * A segment of text may have any of the following states depending on the context.
 **/
enum CodeDiffState
{
	ADD,
	DELETE,
	SAME,
	ADD_DELETE,
	SAME_DELETE,
	ADD_SAME,
	ADD_SAME_DELETED
};

struct CodeDiffFragment
{
	std::string fragment;
	CodeDiffState state;
};

class CodeDiff
{
public:
	std::string oldCode;
	std::string newCode;
	std::vector<CodeDiffFragment> diffCode;

	CodeDiff(std::string oldCode, std::string newCode);
	CodeDiff(std::vector<CodeDiffFragment> diffCode);
	CodeDiff CodeDiffSubstring(int start, int end);//takes the start and end of the newCode

	std::vector<CodeDiffFragment> diff(std::string old, std::string current);

private:
	CodeDiffFragment fragmentFromDtl(dtl::edit_t type, std::string fragment);
};

