#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

std::vector<std::string> tokenise(std::string code)
{
    std::vector<std::string> tokens;
    size_t pos = 0, lastPos = 0;
    while ((pos = code.find_first_of("() ;,|\n", lastPos)) != std::string::npos)
    {
        if(lastPos != pos) tokens.push_back(code.substr(lastPos, pos - lastPos));
        tokens.push_back(code.substr(pos, 1));
        lastPos = pos + 1;
    }
    tokens.push_back(code.substr(lastPos));

    //condense comment tokens
    std::string condensed = "";
    std::string closingToken = "";
    bool found = false;
    std::vector<std::string> result;
    for(auto token : tokens)
    {
    	if(found == true)
    	{
    		if(closingToken == token) 
    		{
    			found = false;
    		 	condensed += token;
    		 	result.push_back(condensed);
    			condensed = "";
    		 	continue;
    		}

    		condensed += token;
    		continue;
    	}

    	if(token == ";")
    	{
    	   	found = true;
    		closingToken = "\n";
    		condensed += token;	
    	}
    	else result.push_back(token);
    }

    return result;
}

std::vector<std::string> getFunctionSubset(std::vector<std::string> subset)
{
	int count = 0;
	int index = 0;
	for(auto token : subset)
	{
		if(token == "(") count++;
		else if(token == ")") count--;
		index++;

		if(count == 0) return std::vector<std::string>(subset.begin(), subset.begin() + index); 
	}

	return std::vector<std::string>();
}

bool isComment(std::string token)
{
	return token.size() > 0 && token[0] == '\"';
}

int countTopLevel(std::vector<std::string> tokens)
{
	int count = 0;
	for(int i = 0; i < tokens.size(); ++i)
	{
		if(tokens[i].find_first_not_of(" \t\n\v\f\r") == std::string::npos) continue;//check if space
		count++;

		if(tokens[i] == "(") i += getFunctionSubset(std::vector<std::string>(tokens.begin() + i, tokens.end())).size() - 1;
	}
	return count;
}

int main(int argc, char** argv)
{
	for(auto token : tokenise("(test function with comment \"string\");;blah this is a comment\n(next line)"))
	{
		std::cout << token << std::endl;
	}

	std::cout << countTopLevel(tokenise("(test function with comment \"string\");;blah this is a comment\n(next line)")) << std::endl;
}

