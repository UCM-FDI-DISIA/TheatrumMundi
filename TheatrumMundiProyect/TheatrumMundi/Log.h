#pragma once

#include <list>
#include <string>
#include "TextInfo.h"

class Log
{
private:
	// a list of pairs to save all showed dialogue lines
	// first: author
	// second: dialogueLine
	std::list<TextInfo> _log;

public:
	
	Log();

	//add new dialogue line to log registry
	void addDialogueLineLog(std::string author, std::string dialogueLine);

	//delete of enitities
	~Log();

	std::list<TextInfo>* getLogList()
	{
		return &_log;
	}



};