#include "Log.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

using namespace std;

Log::Log()
{
	addDialogueLineLog("/", "/");
}

//add one dialogueLine (with its author) on log registry
void Log::addDialogueLineLog(string author, string dialogueLine)
{
	pair<string, string> newLine;
	newLine.first = author;
	newLine.second = dialogueLine;
	_log.push_back(newLine);
}

// show ingame registry log
// temporarly cout
void Log::showLog()
{
	for (auto a : _log)
	{
		if (a.first == "/" && a.second == "/") // show separation of dialogues
		{
			cout << "/....../....../......./" << endl;
		}
		else
		{
			cout << a.first << " " << a.second << " " << endl;
		}
		
	};
}

Log::~Log()
{
}
