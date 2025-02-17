#include "Log.h"

#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"

using namespace std;

Log::Log()
{
	//addDialogueLineLog("/", "/");
}

/*
//add one dialogueLine (with its author) on log registry
void Log::addDialogueLineLog(string author, string dialogueLine)
{
	pair<string, string> newLine;
	newLine.first = author;
	newLine.second = dialogueLine;
	_log.push_back(newLine);
}*/



void Log::addDialogueLineLog(std::string dialogueEv, const pair<string, string>& dialogueLine)
{
	//first: insert if dialogueEv has not been displayed before
	// if dialogueEv is already registered add new dialogueLines to its list
	list<pair<string, string>> newlist;

	auto result = _log.insert({ dialogueEv, newlist });

	//result.first == std::pair<clave, valor>
	// valor == lista de dialogos

	if (!result.second) //if the event already exists, only insert line if the event is not finished
	{
		if (result.first->second.back().second != "/")
		{
			result.first->second.push_back(dialogueLine);
		}
	}
	else //if new event insert in order vector and insert first line
	{
		result.first->second.push_back(dialogueLine);
		_dialogueOrder.push_back(dialogueEv);
	}

}

// show ingame registry log
// temporarly cout
void Log::showLog()
{
	/*
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
	*/

	cout << "/....../....../......./" << endl;

	for (auto a : _dialogueOrder)
	{
		auto it = _log.find(a); // searches for the dialogueEv
		
		for (auto a : it->second) // shows every dialogue Line of the searched dialogueEv
		{
			if (a.second != "/")
			{
				cout << a.first << " " << a.second << " " << endl;
			}

		};

		cout << "/....../....../......./" << endl;
	};
}

Log::~Log()
{
}
