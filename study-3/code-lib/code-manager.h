#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/**
 * A single line in code.
 */
struct CodeLine
{
	int id;//unique identifier for this code line
	int lineCount;//global line index
	int startChar;//global start character index
	std::string code;
	std::string codePrevious;

	bool isSelected;//is this code line selected?
	bool isActive;
	bool isError;
};

/**
 * Code concept. eg. a function
 */
struct CodeState
{
	int id;//unique identifier for this code state
	int index;//global index of the state. eg. first function will be 0
	std::vector<CodeLine> lines;

	bool isSelected;//is this code state selected
	bool isActive;
	bool isError;
};

class CodeManager
{
public:
	CodeManager();
	~CodeManager();

	void update(std::string code);//updates the offline code
	void evaluate(std::string code);//updates the online code
	void select(int selection);//simple selection
	void select(int selection, int selectionEnd);//simple selection
	void error(std::string message);//error from evaluation

	std::vector<CodeState> states;
	std::string code;//most recent code
	std::string evaluation;//most recent evaluation... code most likely to cause error	


	CodeState* updateState(std::string line, int index, int stateCount, int lineCount, int charCount);//returns the updated state
	void updateLine(CodeState* state, std::string line, int index, int lineCount, int charCount);
	CodeState* find(std::string line);
	bool similar(std::string str1, std::string str2);
};

extern "C" 
{
	struct code_manager;//used by interface-manager
	struct code_state;//corresponds to widget
	struct code_line;//corresponds to widget child attractor

	//Code Manager
	code_manager* code_manager_create();
	void code_manager_destroy(code_manager* manager);

	void code_manager_update(code_manager* manager, char* code);
	void code_manager_evaluate(code_manager* manager, char* code);
	void code_manager_select(code_manager* manager, int selection);
	void code_manager_select_2(code_manager* manager, int selectionStart, int selectionEnd);
	void code_manager_error(code_manager* manager, char* message);

	int code_manager_states_count(code_manager* manager);
	code_state* code_manager_states_get(code_manager* manager, int index);
	
	//Code State
	int code_state_id_get(code_state* state);
	int code_state_lines_count(code_state* state);
	int code_state_get_index(code_state* state);
	code_line* code_state_lines_get(code_state* state, int index);
	bool code_state_is_selected(code_state* state);
	bool code_state_is_active(code_state* state);
	bool code_state_is_error(code_state* state);

	//Code Line
	int code_line_id_get(code_line* line);
	const char* code_line_get_code(code_line* line);
	bool code_line_is_selected(code_line* line);
	bool code_line_is_active(code_line* line);
	bool code_line_is_error(code_line* line);
}

void printStates(CodeManager* manager)
{
	for(auto & state : manager->states)
	{
		std::cout << "state: " << state.id << std::endl;
		std::cout << "line count: " << state.lines.size() << std::endl;
		for(auto & line : state.lines)
		{
			std::cout << line.code.length() << std::endl;
		}
		
	}
}

int main(int argc, char* argv[])
{
	CodeManager manager;

	bool similar(std::string str1, std::string str2);

	if(manager.similar("test2 test2", "test1 test2 test5")) std::cout << "similar" << std::endl;
	else std::cout << "different" << std::endl;

	std::string code1 = "(test function\n here)\n\n(another3 test \nfunction)\n";
	manager.update(code1);

	printStates(&manager);

	std::cout << "------------------" << std::endl;

	std::string code2 = "(test function\n here)\n\n(another3 test \nfunction)\n\n\n\n\n";
	manager.update(code1);

	manager.evaluate(code1);
	// if(manager.states[0].isActive) std::cout << "is active" << std::endl;

	printStates(&manager);

	std::cout << "------------------" << std::endl;

	std::string code3 = "(single function)\n";
	manager.update(code1);
	printStates(&manager);

}





