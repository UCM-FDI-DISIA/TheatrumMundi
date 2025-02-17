#pragma once

#include <list>
#include <string>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"

class SceneTemplate;

using namespace ecs;
class Log
{
private:
	// a list of pairs to save all showed dialogue lines
	// first: author
	// second: dialogueLine
	std::list<std::pair<std::string, std::string>> _log;
protected:
public:
	Log();
	
	//add new dialogue line to log registry
	void addDialogueLineLog(std::string author, std::string dialogueLine);

	//show log (dialogueManager)
	void showLog(); //temporarly cout

	//delete of enitities
	~Log();

};
