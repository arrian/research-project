#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <boost/tokenizer.hpp>

using namespace boost;

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
		if(this->code != code) isActive = false;
		this->code = code;
		this->lineCount = line;
		this->startChar = index;	
	}
};

struct Code
{
	int id;//index of this code
	std::string code;
	int startLine;
	int startIndex;

	Code(int id, std::string code, int startLine, int startIndex)
	{
		this->id = id;
		this->code = code;
		this->startLine = startLine;
		this->startIndex = startIndex;
	}

	bool isDeactivatedDefine()
	{
		if(std::count(code.begin(), code.end(), '\n') > 0) return false;
		int i = 0;
		std::string define = "(define ";
		while(i < define.length() && i < code.length())
		{
			if(define[i] != code[i]) return false;
			i++;
		}
		if(i <= define.length() && code[code.length() - 1] == ')') return true;
		return false;
	}
};

struct Function
{
	int id;//unique identifier for this function
	int index;//global index of the function. eg. first function will be 0
	std::vector<Line> lines;
	Code code;

	bool isSelected;//is this code function selected
	bool isActive;//has this function been evaluated
	bool isKilled;//is this function of the form '(define function)...'
	bool isError;//has an error ocurred during evaluation

	Function(Code code)
		: code(code)
	{
		this->id = getFunctionIdentifier();
		this->index = code.id;
		this->isSelected = false;
		this->isActive = false;
		this->isKilled = false;
		this->isError = false;

		int currentLine = code.startLine;
		int currentIndex = code.startIndex;
		char_separator<char> sep("\n\r");
		tokenizer<char_separator<char>> tokens(code.code, sep);
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
		if(lines.size() > 0) return getName(lines[0].code);
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

	bool equal(Code code)
	{
		if(this->code.code == code.code) return true;

		if(lines.size() > 0 && lines[0].code == code.code) return true;//only first line is match

		return false;
	}

	/**
	 * Returns true if this function is a calling function. 
	 * eg. the function (example 10) for (define example...)
	 */
	bool isCaller(Code code)
	{
		std::string name = getName(code.code);
		unsigned pos = name.find_last_of(" ");
		// std::cout << "comparing " << getName() << " with " << name.substr(pos + 1) << std::endl;
		if(pos != std::string::npos) return getName() == name.substr(pos + 1);

		return false;
	}

	bool nameEquals(std::string name)
	{
		// std::cout << "comparing " << getName() << " and " << getName(name) << std::endl;
		return (getName() == getName(name));
	}

	bool isDeactivated(Code code)
	{
		if(lines.size() == 0) return true;

		for(int i = 0; i < lines[0].code.length(); i++)
		{
			if(lines[0].code[i] == ')') return true;
		}
		return false;
	}


	/**
	 * Returns true if the function was successfully updated.
	 */
	bool update(Code code)
	{
		if(!nameEquals(getName(code.code))) return false;//TODO: could have a smarter heuristic here

		std::vector<Line> newLines;
		this->code = code;
		int currentLine = code.startLine;
		int currentIndex = code.startIndex;
		int count = 0;
		char_separator<char> sep("\n\r");
		tokenizer<char_separator<char>> tokens(code.code, sep);
		for (const auto& t : tokens)
		{
			currentLine++;
			currentIndex += t.size();
			count++;
			if(lines.size() < count)
			{
				Line line(t.substr(0, t.find(";")), currentLine, currentIndex);
				newLines.push_back(line);
			}
			else
			{
				lines[count - 1].update(t.substr(0, t.find(";")), currentLine, currentIndex);
				// lines[count - 1].isActive = false;
				newLines.push_back(lines[count - 1]);
			}
		}

		lines = newLines;
		
		return true;//was updated
	}

	bool contains(int index)
	{
		return (index >= code.startIndex && index <= code.startIndex + code.code.length());
	}

	void setSelected(bool selected)
	{
		this->isSelected = selected;
	}
};

class CodeManager
{
public:
	CodeManager()
	{
		this->code = "";
		this->evaluation = "";
	}

	~CodeManager() {}

	//updates the offline code
	void update(std::string code)
	{
		std::vector<Function> newFunctions;
		std::vector<Code> split = splitFunctions(code);
		for(auto & c : split)
		{
			bool done = false;

			for(auto & f : functions)
			{
				done = f.update(c);//attempt an update on all existing functions
				if(done) 
				{
					newFunctions.push_back(f);
					break;
				}
			}

			if(!done) newFunctions.push_back(Function(c));
		}

		functions = newFunctions;
	}

	//updates the online code
	void evaluate(std::string code)
	{
		std::vector<Code> split = splitFunctions(code);
		
		for(auto & c : split)
		{
			for(auto & f : functions)
			{
				if(f.equal(c) || (c.isDeactivatedDefine() && f.isCaller(c))) 
				{
					bool activeState = !c.isDeactivatedDefine();
					f.isActive = activeState;

					for(auto & l : f.lines)
					{
						l.isActive = activeState;
					}
				}
			}
		}
	}
	void select(int selection){}//simple selection
	void select(int selection, int selectionEnd){}
	void error(std::string message){}//error from evaluation
	void cursor(int selection, int screenMin, int screenMax, int xPosition, int yPosition)
	{
		for(auto & f : functions)
		{
			f.setSelected(f.contains(selection));
		}
		screen.selection = selection;
		screen.screenMin = screenMin;
		screen.screenMax = screenMax;
		screen.xPosition = xPosition;
		screen.yPosition = yPosition;
	}

	Screen screen;
	std::vector<Function> functions;
	std::string code;//most recent code
	std::string evaluation;//most recent evaluation
	std::string message;//most recent evaluation message

	std::vector<Code> splitFunctions(std::string code)
	{
		std::vector<Code> results;
		int initial = 0;
		int count = 0;
		int depth = 0;
		int lineCount = 0;
		int index = 0;
		bool commented = false;
		for(int i = 0; i < code.length(); i++)
		{
			count++;
			if(code[i] == '\n')
			{
				commented = false;
				lineCount++;
			}
			else if(code[i] == ';' || commented)
			{
				commented = true;
				continue;
			}
			else if(code[i] == '(')
			{
				if(depth == 0) 
				{
					initial = i;
					count = 1;
				}
				depth++;
			}
			else if(code[i] == ')') 
			{
				depth--;
				if(depth <= 0)
				{
					depth = 0;
					// if(results.size() > 0 && results.back().isDeactivatedDefine()) std::cout << "was isDeactivated" << std::endl;
					if(results.size() > 0 && results.back().isDeactivatedDefine())
					{
						// std::cout << "l = " << code.length() << " si = " << results.back().startIndex << " cl = " << results.back().code.length() << " count = " << count << std::endl; 
						results.back().code += code.substr(results.back().startIndex + results.back().code.length(), count);
					}
					else
					{
						results.push_back(Code(index, code.substr(initial, count), lineCount, initial));
						if(results.back().isDeactivatedDefine()) depth = 1;

						index++;
					}

					count = 0;					
				}
			}
		}
		return results;
	}

	// Function* updateFunction(std::string line, int index, int functionCount, int lineCount, int charCount);//returns the updated function
	// void updateLine(Function* function, std::string line, int index, int lineCount, int charCount);
	// Function* find(std::string line);
	// bool similar(std::string str1, std::string str2);

	// bool similar(Function* function1, Function* function2);
	// void update(Function* target, Function* with);
	// Function* find(std::string name);

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
	
	int code_manager_get_cursor_selection(code_manager* manager);
	int code_manager_get_cursor_x(code_manager* manager);
	int code_manager_get_cursor_y(code_manager* manager);
	int code_manager_get_screen_min(code_manager* manager);//minimum visible character
	int code_manager_get_screen_max(code_manager* manager);//maximum visible character

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

void evaluate_and_print(code_manager* manager)
{
	printf("functions: %d\n", code_manager_functions_count(manager));
	for(int i = 0; i < code_manager_functions_count(manager); i++) printf("function %d with id %d: %s\n", i, function_get_id(code_manager_functions_get(manager, i)), line_get_code(function_lines_get(code_manager_functions_get(manager, i), 0)));

}

int main(int argc, char* argv[])
{
	code_manager* manager = code_manager_create();

	// evaluate_and_print(manager, "(test function here)");
	// evaluate_and_print(manager, "(test function here2)");
	// evaluate_and_print(manager, "(test function here3)");
	// evaluate_and_print(manager, "(test3 function here)");
	// evaluate_and_print(manager, "(test function here)(test function here)");
	// evaluate_and_print(manager, "(test (((((((function)))) here))));;testing comments");

	// evaluate_and_print(manager, "(define speaker\n  (lambda (beat dur i)\n    (play speech (* 10 (+ i 1)) (cosr 50 20 1/2) dur 2)\n    (callback (*metro* (+ beat (* .5 dur))) 'speaker (+ beat dur)\n              dur\n              (if (< i 6) (+ i 1) 0))))\n\n(speaker (*metro* 'get-beat 4) 1/4 0)\n\n\n(define drums\n  (lambda (beat dur)\n    (play kit (cosr 43 3 1/8) (cosr 60 30 1) .1)\n    (play kit (cosr 53 7 1/7) (cosr 60 30 1) .1)\n    (play kit *gm-open-hi-hat* (cosr 70 10 2) .1 1)\n    (if (= (modulo beat 1) 0) (play kit *gm-kick* 110 .1 1))\n    (if (= (modulo beat 2) 1) (play kit *gm-snare-2* 110 .1 1))\n    (callback (*metro* (+ beat (* .5 dur))) 'drums (+ beat dur) dur)))\n\n(drums (*metro* 'get-beat 4) 1/4)\n\n\n\n\n;; cheers\n;;\n;; ben.swift@anu.edu.au\n;; http://benswift.me\n\n");
	// code_manager_update(manager, "(define function)\ntesting)(more here)\n(testing more)");
	// evaluate_and_print(manager);
	// code_manager_update(manager, "(define function\ntesting)(more here)\n(testing more)");
	// evaluate_and_print(manager);


	// Code code1(0, "(define test \nfunction)", 0, 0);
	// Code code2(1, "(test2 function)", 0, 0);

	// Function func(code2);

	// if(func.isCaller(code1)) std::cout << "is caller" << std::endl;
	// else std::cout << "not caller" << std::endl;

	// testing commenting
	code_manager_update(manager, "(define test \nfunction)");
	code_manager_update(manager, ";;(define test \n;;function)");
	printf("functions: %d\n", code_manager_functions_count(manager));

	return 0;
}





