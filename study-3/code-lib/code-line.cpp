#include "code-line.h"


CodeSimple::CodeSimple(std::string code)
{
	lines = split(code, "\n");
}

std::string CodeSimple::getCode()
{
	std::string s = "";

	for(int i = 0; i < lines.size(); i++)
	{
		s += lines[i];
	}

	return s;
}

int CodeSimple::getLineCount()
{
	return lines.size();
}

std::string CodeSimple::getLineAt(int index)
{
	if(index >= lines.size()) return "";
	return lines[index];
}

bool CodeSimple::isFirstLineMatch(std::string other)
{
	if(lines.size() < 1) return false;
	std::vector<std::string> os = split(other, "\n");
	if(os.size() < 1) return false;
	return lines[0] == os[0];
}

bool CodeSimple::isFirstLineVoid()
{
	return false;//TODO: implement
}

std::vector<std::string> CodeSimple::split(std::string s, std::string delim) {
	std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = s.find(delim, prev)) != std::string::npos)
    {
        strings.push_back(s.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(s.substr(prev));

    return strings;
}
