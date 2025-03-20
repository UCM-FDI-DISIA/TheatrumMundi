#include "Log.h"

Log::Log()
{
}

//adds one dialogueLine (with its author) on log registry
void Log::addDialogueLineLog(std::string author, std::string dialogueLine)
{
	TextInfo aux;
	aux.Character = author;
	aux.Text = dialogueLine;
	_log.push_front(aux);
}

Log::~Log()
{
}

void Log::SetLogActive(bool logActive)
{
	_logActive = logActive;
}

bool Log::GetLogActive()
{
	return _logActive;
}
