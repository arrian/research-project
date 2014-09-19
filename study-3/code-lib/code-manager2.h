#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define DEFINE "define"
#define BIND_FUNC "bind-func"

int getLineIdentifier()
{  
    static int LINE_IDENTIFIER = 0;
    return ++LINE_IDENTIFIER;
}

int getFunctionIdentifier()
{  
    static int STATE_IDENTIFIER = 0;
    return ++STATE_IDENTIFIER;
}


struct Screen
{
	int selection;
	int screenMin;
	int screenMax;
	int xPosition;
	int yPosition;
};

/**
 * A single line in code.
 */
struct Line
{
	int id;//unique identifier for this code line
	int lineCount;//global line index
	int startChar;//global start character index
	std::string code;
	std::string codePrevious;

	bool isSelected;//is this code line selected?
	bool isActive;
	bool isError;

	Line(std::string code, int line, int index)
	{
		this->id = getLineIdentifier();
		this->codePrevious = "";
		this->isSelected = false;
		this->isActive = false;
		this->isError = false;
		this->code = code;
		this->lineCount = line;
		this->startChar = index;
	}

	void update(std::string code, int line, int index)
	{
		this->code = code;
		this->lineCount = line;
		this->startChar = index;	
	}
};

struct Function
{
	int id;//unique identifier for this function
	int index;//global index of the function. eg. first function will be 0
	std::vector<Line> lines;

	bool isSelected;//is this code function selected
	bool isActive;//has this function been evaluated
	bool isKilled;//is this function of the form '(define function)...'
	bool isError;//has an error ocurred during evaluation

	Function(std::string code, int startLine, int startIndex)
	{
		this->id = getFunctionIdentifier();
		this->isSelected = false;
		this->isActive = false;
		this->isKilled = false;
		this->isError = false;

		int currentLine = startLine;
		int currentIndex = startIndex;
		char_separator<char> sep("\n\r");
		tokenizer<char_separator<char>> tokens(code, sep);
		for (const auto& t : tokens) {
			currentLine++;
			currentIndex++;
			if(t.empty()) continue;

			Line line(t.substr(0, t.find(";")), currentLine, currentIndex);
			lines.push_back(line);

			currentIndex += t.size();//this index will be off by at least 1. TODO: fix
		}
	}

	std::string getName()
	{
		if(lines.size() > 0) return getName(lines[0]);
		return "";
	}

	std::string getName(std::string code)
	{
		std::string result = "";
		char_separator<char> sep(" ():\n\t\r");
		tokenizer<char_separator<char>> tokens(code, sep);
		for (const auto& t : tokens)
		{
			if(t == DEFINE || t == BIND_FUNC) result += t + " ";
			else
			{
				result += t;
				return result;
			}
		}
		return result;
	}

	/**
	 * Returns true if the function was successfully updated.
	 */
	bool update(std::string code, int startLine, int startIndex)
	{
		if(getName() != getName(code)) return false;//TODO: could have a smarter heuristic here

		int currentLine = startLine;
		int currentIndex = startIndex;
		int count = 0;
		char_separator<char> sep("\n\r");
		tokenizer<char_separator<char>> tokens(code, sep);
		for (const auto& t : tokens)
		{
			currentLine++;
			currentIndex += t.size();
			count++;
			if(lines.size() < count)
			{
				Line line(t.substr(0, t.find(";")), );
				lines.push_back(line);
			}
			else
			{
				lines[count - 1].update(t.substr(0, t.find(";")), currentLine, currentIndex);
			}
		}
		
		return true;//was updated
	}
};



class CodeManager
{
public:
	CodeManager();
	~CodeManager();

	void update(std::string code);//updates the offline code
	void evaluate(std::string code);//updates the online code
	void select(int selection);//simple selection
	void select(int selection, int selectionEnd);
	void error(std::string message);//error from evaluation
	void cursor(int selection, int screenMin, int screenMax, int xPosition, int yPosition);

	Screen screen;
	std::vector<Function> functions;
	std::string code;//most recent code
	std::string evaluation;//most recent evaluation
	std::string message;//most recent evaluation message

	std::vector<std::string> splitFunctions(std::string code)
	{
		std::vector<std::string> results;
		int initial = 0;
		int count = 0;
		int depth = 0;
		for(int i = 0; i < code.length(); i++)
		{
			if(code[i] == "(")
			{
				if(depth == 0) initial = i;
				depth++;
			}
			else if(code[i] == ")") 
			{
				depth--;
				if(depth <= 0)
				{
					count++;
					results.push_back(code.substr(intial, count));
					depth = 0;
					count = 0;
				}
			}
			else count++;
		}
	}

	Function* updateFunction(std::string line, int index, int functionCount, int lineCount, int charCount);//returns the updated function
	void updateLine(Function* function, std::string line, int index, int lineCount, int charCount);
	Function* find(std::string line);
	bool similar(std::string str1, std::string str2);

	bool similar(Function* function1, Function* function2);
	void update(Function* target, Function* with);
	Function* find(std::string name);

};


extern "C" 
{
	struct code_manager;//used by interface-manager
	struct function;//corresponds to widget
	struct line;//corresponds to widget child attractor
	struct screen;

	//Code Manager
	code_manager* code_manager_create();
	void code_manager_destroy(code_manager* manager);

	void code_manager_update(code_manager* manager, char* code);
	void code_manager_evaluate(code_manager* manager, char* code);
	void code_manager_error(code_manager* manager, char* message);
	void code_manager_cursor(code_manager* manager, int cursorPosition, int screenMin, int screenMax, int xPosition, int yPosition);

	int code_manager_functions_count(code_manager* manager);
	function* code_manager_functions_get(code_manager* manager, int index);
	
	//Code State
	int function_get_id(function* f);
	int function_lines_count(function* f);
	int function_get_index(function* f);
	line* function_lines_get(function* f, int index);
	bool function_is_selected(function* f);
	bool function_is_active(function* f);
	bool function_is_error(function* f);

	//Code Line
	int line_get_id(line* l);
	const char* line_get_code(line* l);
	bool line_is_selected(line* l);
	bool line_is_active(line* l);
	bool line_is_error(line* l);


}







