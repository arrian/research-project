#include "code-manager.h"

#include <iostream>
#include <algorithm>


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

void code_manager_error(code_manager* manager, char* message)
{
	reinterpret_cast<CodeManager*>(manager)->error(std::string(message));
}

void code_manager_cursor(code_manager* manager, int cursorPosition, int screenMin, int screenMax, int xPosition, int yPosition)
{
	reinterpret_cast<CodeManager*>(manager)->cursor(cursorPosition, screenMin, screenMax, xPosition, yPosition);
}

void code_manager_callback(code_manager* manager, char* name, int next)
{
	reinterpret_cast<CodeManager*>(manager)->callback(std::string(name), next);
}

int code_manager_functions_count(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->functions.size();
}

function* code_manager_functions_get(code_manager* manager, int index)
{
	return reinterpret_cast<function*>(&reinterpret_cast<CodeManager*>(manager)->functions[index]);
}

int code_manager_get_cursor_selection(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->screen.selection;
}

int code_manager_get_cursor_x(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->screen.xPosition;
}

int code_manager_get_cursor_y(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->screen.yPosition;
}

int code_manager_get_screen_min(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->screen.screenMin;
}

int code_manager_get_screen_max(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->screen.screenMax;
}

int function_get_id(function* f)
{
	return reinterpret_cast<Function*>(f)->id;
}

int function_lines_count(function* f)
{
	if(!reinterpret_cast<Function*>(f)->isDefine()) return 0;//we only care about visualising the important stuff
	return reinterpret_cast<Function*>(f)->lines.size();
}

int function_get_index(function* f)
{
	return reinterpret_cast<Function*>(f)->index;
}

line* function_lines_get(function* f, int index)
{
	return reinterpret_cast<line*>(&reinterpret_cast<Function*>(f)->lines[index]);
}

bool function_is_selected(function* f)
{
	return reinterpret_cast<Function*>(f)->isSelected;
}

bool function_is_active(function* f)
{
	return reinterpret_cast<Function*>(f)->isActive;
}

bool function_is_error(function* f)
{
	return reinterpret_cast<Function*>(f)->isError;
}

bool function_is_define(function* f)
{
	return reinterpret_cast<Function*>(f)->isDefine();
}

int function_get_next(function* f, int now)
{
	return reinterpret_cast<Function*>(f)->getNext(now);
}

int line_get_id(line* l)
{
	return reinterpret_cast<Line*>(l)->id;
}

const char* line_get_code(line* l)
{
	return reinterpret_cast<Line*>(l)->code.c_str();	
}

bool line_is_selected(line* l)
{
	return reinterpret_cast<Line*>(l)->isSelected;
}

bool line_is_active(line* l)
{
	return reinterpret_cast<Line*>(l)->isActive;
}

bool line_is_error(line* l)
{
	return reinterpret_cast<Line*>(l)->isError;
}



