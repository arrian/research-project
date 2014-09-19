#include "code-manager2.h"

#include <iostream>
#include <algorithm>


CodeManager::CodeManager()
	: code(""),
	  evaluation("")
{
}

CodeManager::~CodeManager()
{
}

void CodeManager::update(std::string code)
{
	int functionCounter = 0;
	int lineCounter = 0;
	CodeFunction* foundFunction = nullptr;
	int foundCounter = 0;
	int charCounter = 0;

	std::vector<CodeFunction> newFunctions;

	std::istringstream codestream(code);
	std::string line;
	while(std::getline(codestream, line))
	{
		if(line.find_first_not_of(" \n\t\r") != std::string::npos)//check if anything but whitespace
		{
			if(!foundFunction) 
			{
				foundFunction = updateFunction(line, foundCounter, functionCounter, lineCounter, charCounter);
				functionCounter++;
			}
			else 
			{
				updateLine(foundFunction, line, foundCounter, lineCounter, charCounter);
			}
			foundCounter++;
		}
		else if(foundFunction)
		{
			newFunctions.push_back(*foundFunction);
			foundFunction = nullptr;
			foundCounter = 0;
		}

	    charCounter += line.length() + 1;
	    lineCounter++;
	}
	if(foundFunction) newFunctions.push_back(*foundFunction);
	functions = newFunctions;
}

void CodeManager::evaluate(std::string code)
{
	std::istringstream codestream(code);
	std::string line;

	if(std::getline(codestream, line))
	{
		Function* function = find(line);
		if(function)
		{
			function->isActive = true;
			for(auto & line : function->lines)
			{
				line.isActive = true;
			}
		}
	}
}

void CodeManager::select(int selection)
{
	select(selection, selection);
}

void CodeManager::select(int selectionStart, int selectionEnd)
{
	for(auto & function : functions)
	{
		function.isSelected = false;
		for(auto & line : function.lines)
		{
			line.isSelected = false;
			int start = line.startChar;
			int end = line.startChar + line.code.length();
			if(start < selectionEnd && end > selectionStart)
			{
				function.isSelected = true;
				line.isSelected = true;
			}
		}
	}
}

void CodeManager::error(std::string message)
{
	this->message = message;
}

void CodeManager::cursor(int selection, int screenMin, int screenMax, int xPosition, int yPosition)
{
	select(selection);

	this->screen.selection = selection;
	this->screen.screenMin = screenMin;
	this->screen.screenMax = screenMax;
	this->screen.xPosition = xPosition;
	this->screen.yPosition = yPosition;
}

CodeFunction* CodeManager::updateFunction(std::string line, int index, int functionCount, int lineCount, int charCount)
{
	CodeFunction* function = find(line);
	if(!function)
	{
		CodeFunction newFunction;
		newFunction.id = getFunctionIdentifier();
		newFunction.index = functionCount;
		newFunction.isSelected = false;
		newFunction.isActive = false;
		newFunction.isError = false;

		functions.push_back(newFunction);
		function = &functions[functions.size() - 1];
	}
	else
	{
		function->index = functionCount;
	}
	updateLine(function, line, index, lineCount, charCount);
	return function;
}

void CodeManager::updateLine(CodeFunction* function, std::string line, int index, int lineCount, int charCount)
{
	if(function->lines.size() < (index + 1))//not enough lines in code function
	{
		CodeLine codeLine;
		codeLine.id = getLineIdentifier();
		codeLine.startChar = charCount;
		codeLine.code = line;
		codeLine.codePrevious = "";
		codeLine.isSelected = false;
		codeLine.isActive = false;
		codeLine.isError = false;

		function->lines.push_back(codeLine);
	}
	else//have enough lines in the code function... need to update existing
	{
		CodeLine* codeLine = &function->lines[index];
		codeLine->startChar = charCount;
		codeLine->codePrevious = codeLine->code;
		codeLine->code = line;
		codeLine->isActive = false;//edited line... force inactive
	}
}

CodeFunction* CodeManager::find(std::string line)
{
	for(auto & function : functions)
	{
		if(function.lines.size() > 0 && similar(line, function.lines[0].code)) return &function;
	}
	return nullptr;
}

bool CodeManager::similar(std::string str1, std::string str2)
{
    std::vector<std::string> words1, words2;
    std::string temp;

    std::stringstream stringstream1(str1);
    while (stringstream1 >> temp)
        words1.push_back(temp);

    std::stringstream stringstream2(str2);
    while (stringstream2 >> temp)
        words2.push_back(temp);

    int indenticalCount = 0;
    int length = std::min(words1.size(), words2.size());

    for(int i = 0; i < length; i++) if(words1[i] == words2[i]) indenticalCount++;

    if(length == 0) return false;
    double ratio = (double)indenticalCount / length;
    return ratio > 0.6;
}

//////////////////////////////////////
code_manager* code_manager_create()
{
	return reinterpret_cast<code_manager*>(new CodeManager());
}

void code_manager_destroy(code_manager* manager)
{
	delete reinterpret_cast<CodeManager*>(manager);
}

void code_manager_update(code_manager* manager, char* code)
{
	reinterpret_cast<CodeManager*>(manager)->update(std::string(code));
}

void code_manager_evaluate(code_manager* manager, char* code)
{
	reinterpret_cast<CodeManager*>(manager)->evaluate(std::string(code));
}

// void code_manager_select(code_manager* manager, int selection)
// {
// 	reinterpret_cast<CodeManager*>(manager)->select(selection);
// }

// void code_manager_select_2(code_manager* manager, int selectionStart, int selectionEnd)
// {
// 	reinterpret_cast<CodeManager*>(manager)->select(selectionStart, selectionEnd);
// }

void code_manager_error(code_manager* manager, char* message)
{
	reinterpret_cast<CodeManager*>(manager)->error(std::string(message));
}

void code_manager_cursor(code_manager* manager, int cursorPosition, int screenMin, int screenMax, int xPosition, int yPosition)
{
	reinterpret_cast<CodeManager*>(manager)->cursor(cursorPosition, screenMin, screenMax, xPosition, yPosition);
}

int code_manager_functions_count(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->functions.size();
}

function* code_manager_functions_get(code_manager* manager, int index)
{
	return reinterpret_cast<function*>(&reinterpret_cast<CodeManager*>(manager)->functions[index]);
}

int function_id_get(function* f)
{
	return reinterpret_cast<CodeFunction*>(f)->id;
}

int function_lines_count(function* f)
{
	return reinterpret_cast<CodeFunction*>(f)->lines.size();
}

int function_get_index(function* f)
{
	return reinterpret_cast<CodeFunction*>(f)->index;
}

line* function_lines_get(function* f, int index)
{
	return reinterpret_cast<line*>(&reinterpret_cast<CodeFunction*>(f)->lines[index]);
}

bool function_is_selected(function* f)
{
	return reinterpret_cast<CodeFunction*>(f)->isSelected;
}

bool function_is_active(function* f)
{
	return reinterpret_cast<CodeFunction*>(f)->isActive;
}

bool function_is_error(function* f)
{
	return reinterpret_cast<CodeFunction*>(f)->isError;
}

int line_get_id(line* l)
{
	return reinterpret_cast<CodeLine*>(l)->id;
}

const char* line_get_code(line* l)
{
	return reinterpret_cast<CodeLine*>(l)->code.c_str();	
}

bool line_is_selected(line* l)
{
	return reinterpret_cast<CodeLine*>(l)->isSelected;
}

bool line_is_active(line* l)
{
	return reinterpret_cast<CodeLine*>(l)->isActive;
}

bool line_is_error(line* l)
{
	return reinterpret_cast<CodeLine*>(l)->isError;
}



