#pragma once

#include <sstream>
#include "dtl/dtl.hpp"


class CodeDiff
{
public:
	std::string diff(std::string old, std::string current);
};

