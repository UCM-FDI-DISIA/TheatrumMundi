#include "Log.h"

Log::Log()
{
}

//adds one dialogueLine (with its author) on log registry
void Log::addDialogueLineLog(std::string author, std::string dialogueLine)
{
	_log.push_front({ author, dialogueLine });
}

Log::~Log()
{
}
