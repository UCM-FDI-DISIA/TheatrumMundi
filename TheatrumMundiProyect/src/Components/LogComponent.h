#pragma once

#include "../ecs/Component.h"

#include <list>
#include <string>


using namespace ecs;

class LogComponent: public ecs::Component
{
private:
	// a list of pairs to save all showed dialogue lines
	// first: author
	// second: dialogueLine
	std::list<std::pair<std::string, std::string>> _log;

	// Stores all spoken dialogue with the same structure as DialogueManager
	//std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>> _log;
	
	//registry of the order in which the dialogue has been displayed
	//std::vector<std::string> _dialogueOrder;


protected:
public:
	__CMPID_DECL__(ecs::cmp::LOG_COMPONENT)

	LogComponent();
	
	//add new dialogue line to log registry
	void addDialogueLineLog(std::string author, std::string dialogueLine);


	//show log (dialogueManager)
	void showLog(); //temporarly cout

	//delete of enitities
	~LogComponent();

	std::list<std::pair<std::string, std::string>>* getLogList()
	{
		return &_log;
	}

	

};
