#include "LogComponent.h"

#include "../sdlutils/SDLUtils.h"

using namespace std;

LogComponent::LogComponent()
{
	_logActive = false;
}

void LogComponent::SetLogActive(bool logActive)
{
	_logActive = logActive;
}

bool LogComponent::GetLogActive()
{
	return _logActive;
}


//adds one dialogueLine (with its author) on log registry
void LogComponent::addDialogueLineLog(string author, string dialogueLine)
{
	_log.push_front({author, dialogueLine});
}


/// <summary>
/// Shows the already displayed dialogue. If called in mid dialogue it will show up until the last line the player has seen.
/// </summary>
void LogComponent::showLog()
{
	if (!_log.empty())
	{
		for (auto a : _log)
		{
			cout << a.first << endl << a.second << endl;
			cout << endl;
		};
	}
	
}

LogComponent::~LogComponent()
{
}
