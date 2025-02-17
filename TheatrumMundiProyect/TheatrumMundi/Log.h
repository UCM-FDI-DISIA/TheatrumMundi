#pragma once

#include <list>
#include <vector>
#include <unordered_map>
#include <string>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"


using namespace ecs;
class Log
{
private:
	// a list of pairs to save all showed dialogue lines
	// first: author
	// second: dialogueLine
	//std::list<std::pair<std::string, std::string>> _log;

	// Stores all spoken dialogue with the same structure as DialogueManager
	std::unordered_map<std::string, std::list<std::pair<std::string, std::string>>> _log;
	
	//registry of the order in which the dialogue has been displayed
	std::vector<std::string> _dialogueOrder;


protected:
public:
	Log();
	
	//add new dialogue line to log registry
	//void addDialogueLineLog(std::string author, std::string dialogueLine);

	void addDialogueLineLog(std::string dialogueEv, const std::pair<std::string,std::string>& dialogueList);

	//show log (dialogueManager)
	void showLog(); //temporarly cout

	//delete of enitities
	~Log();

};
