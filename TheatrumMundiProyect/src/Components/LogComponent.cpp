#include "LogComponent.h"

#include "../sdlutils/SDLUtils.h"

using namespace std;

LogComponent::LogComponent()
{
}


//add one dialogueLine (with its author) on log registry
void LogComponent::addDialogueLineLog(string author, string dialogueLine)
{
	pair<string, string> newLine;
	newLine.first = author;
	newLine.second = dialogueLine;
	_log.push_front(newLine);
}

/*
/// <summary>
/// Adds new dialogueLine to log. It needs to be called right after a dialogueLine is displayed.
/// </summary>
/// <param name="dialogueEv">Name of the dialogue event</param>
/// <param name="dialogueLine">Pair (later TextInfo) to insert on log</param>

void LogComponent::addDialogueLineLog(std::string dialogueEv, const pair<string, string>& dialogueLine)
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

}*/


/// <summary>
/// Shows the already displayed dialogue. If called in mid dialogue it will show up until the last line the player has seen.
/// </summary>
void LogComponent::showLog()
{
	if (!_log.empty())
	{
		for (auto a : _log)
		{
			cout << a.first << " " << a.second << " " << endl;
		};
	}
	
	/*
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
	*/
}

LogComponent::~LogComponent()
{
}
