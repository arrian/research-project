#include "code-manager.h"

int getIdentifier()
{  
    static int IDENTIFIER = 0;
    return ++IDENTIFIER;
}

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
	int lineCounter = 0;
	CodeState* foundState = nullptr;
	int foundCounter = 0;

	int charCounter = 0;

	std::vector<CodeState> newStates;

	std::istringstream codestream(code);
	std::string line;
	while(std::getline(codestream, line))
	{
		if(line.find_first_not_of(' ') != std::string::npos)//check if anything but whitespace
		{
			if(!foundState) foundState = updateState(line, foundCounter, charCounter);
			else updateLine(foundState, line, foundCounter, charCounter);
			foundCounter++;
		}
		else
		{
			newStates.push_back(*foundState);
			foundState = nullptr;
			foundCounter = 0;
		}

	    charCounter += line.length();
	    lineCounter++;
	}
	if(foundState) newStates.push_back(*foundState);
	states = newStates;
}

void CodeManager::evaluate(std::string code)
{
	std::istringstream codestream(code);
	std::string line;

	if(std::getline(codestream, line))
	{
		CodeState* state = find(line);
		if(state)
		{
			state->isActive = true;
			for(auto & line : state->lines)
			{
				line.isActive = true;
			}
		}
	}
}

void CodeManager::select(int selection)
{
	for(auto & state : states)
	{
		state.isSelected = false;
		for(auto & line : state.lines)
		{
			line.isSelected = false;
			int start = line.startChar;
			int end = line.startChar + line.code.length();
			if(selection >= start && selection <= end)
			{
				state.isSelected = true;
				line.isSelected = true;
			}
		}
	}
	
}

void CodeManager::error(std::string message)
{

}

CodeState* CodeManager::updateState(std::string line, int index, int charCount)
{
	CodeState* state = find(line);
	if(!state)
	{
		CodeState newState;
		newState.id = getIdentifier();
		newState.isSelected = false;
		newState.isActive = false;
		newState.isError = false;

		states.push_back(newState);
		state = &states[states.size() - 1];
	}
	updateLine(state, line, index, charCount);
	return state;
}

void CodeManager::updateLine(CodeState* state, std::string line, int index, int charCount)
{
	if(state->lines.size() < (index + 1))//not enough lines in code state
	{
		CodeLine codeLine;
		codeLine.id = getIdentifier();
		codeLine.startChar = charCount;
		codeLine.code = line;
		codeLine.isSelected = false;
		codeLine.isActive = false;
		codeLine.isError = false;

		state->lines.push_back(codeLine);
	}
	else//have enough lines in the code state... need to update existing
	{
		CodeLine* codeLine = &state->lines[index];
		codeLine->startChar = charCount;
		codeLine->code = line;
		codeLine->isActive = false;//edited line... force inactive
	}
}

CodeState* CodeManager::find(std::string line)
{
	for(auto & state : states)
	{
		if(state.lines.size() > 0 && similar(line, state.lines[0].code)) return &state;
	}
	return nullptr;
}

bool CodeManager::similar(std::string str1, std::string str2)
{
	std::cout << "checking similarity of" << str1 << " and " << str2 << std::endl;

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
    if(ratio > 0.6) std::cout << "similar" << std::endl;
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

void code_manager_select(code_manager* manager, int selection)
{
	reinterpret_cast<CodeManager*>(manager)->select(selection);
}

void code_manager_error(code_manager* manager, char* message)
{
	reinterpret_cast<CodeManager*>(manager)->error(std::string(message));
}

int code_manager_states_count(code_manager* manager)
{
	return reinterpret_cast<CodeManager*>(manager)->states.size();
}

code_state* code_manager_states_get(code_manager* manager, int index)
{
	return reinterpret_cast<code_state*>(&reinterpret_cast<CodeManager*>(manager)->states[index]);
}

int code_state_id_get(code_state* state)
{
	return reinterpret_cast<CodeState*>(state)->id;
}

int code_state_lines_count(code_state* state)
{
	return reinterpret_cast<CodeState*>(state)->lines.size();
}

code_line* code_state_lines_get(code_state* state, int index)
{
	return reinterpret_cast<code_line*>(&reinterpret_cast<CodeState*>(state)->lines[index]);
}

bool code_state_is_selected(code_state* state)
{
	return reinterpret_cast<CodeState*>(state)->isSelected;
}

bool code_state_is_active(code_state* state)
{
	return reinterpret_cast<CodeState*>(state)->isActive;
}

bool code_state_is_error(code_state* state)
{
	return reinterpret_cast<CodeState*>(state)->isError;
}

int code_line_id_get(code_line* line)
{
	return reinterpret_cast<CodeLine*>(line)->id;
}

// char* code_line_get_code(code_line* line)
// {
		// return reinterpret_cast<CodeLine*>(line).code.c_str();	
// }

bool code_line_is_selected(code_line* line)
{
	return reinterpret_cast<CodeLine*>(line)->isSelected;
}

bool code_line_is_active(code_line* line)
{
	return reinterpret_cast<CodeLine*>(line)->isActive;
}

bool code_line_is_error(code_line* line)
{
	return reinterpret_cast<CodeLine*>(line)->isError;
}



