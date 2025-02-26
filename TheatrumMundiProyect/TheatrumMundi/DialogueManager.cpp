#include "DialogueManager.h"
#include <fstream>
#include <cassert>
#include "../src/json/json.hpp";
//#include "../src/Components/WriteTextComponent.h";
//#include "TextInfo.h"

#include "../src/components/LogComponent.h"


using json = nlohmann::json;

using namespace std;

/// <summary>
/// Fills the Dialogue map with all the dialogue Events for each room
/// </summary>
void DialogueManager::ReadJson(){

	//Open de json path

	ifstream mJson("../resources/config/dialogues.json");
	assert(mJson);
	json dialogues;
    mJson >> dialogues;
	
	//set room dialogues
	for (int i = 1; i <= numRooms; ++i) {

		string room = "Sala" + to_string(i);
		assert(dialogues.contains(room));
		RoomDialogues r;

		//Set all events from the specific room
		for (auto& elem : dialogues[room].items()) {

			//Fill r with all the evenet dialogues 
			for (auto& elem2 : elem.value()) {

				string character = elem2["Character"];
				string text = elem2["Text"];
				r[elem.key()].push_back(TextInfo{ character,text });
			}
		}

		//Upload Map
		mRoom[room] = r;
	}

}


DialogueManager::DialogueManager() : _sceneLog(nullptr), _writeTextComp(nullptr){

	actualroom = 1;
	room = "Sala" + to_string(actualroom);
	//Load Json in the dialogue map
	ReadJson();

	_showText = new TextInfo{ " " , " " }; //initialize showing text

}

/// <summary>
/// Parsed the event to string so the map can read it
/// </summary>
/// <param name="event"></param> --> Event parsed
/// <param name="_eventToRead"></param> --> Event to parse
void DialogueManager::ParseEnum(string& event, const eventToRead& _eventToRead) {

	switch (_eventToRead) {
	case(SalaIntermediaEvento1):
		event = "SalaIntermediaEvento1";
		break;
	case(SalaIntermediaEvento2):
		event = "SalaIntermediaEvento2";
		break;
	case(SalaIntermediaEvento3):
		event = "SalaIntermediaEvento3";
		break;
	case(Pista1):
		event = "Pista1";
		break;
	case(Pista2):
		event = "Pista2";
		break;
	case(Pista3):
		event = "Pista3";
		break;
	case(Puzzle1):
		event = "Puzzle1";
		break;
	case(Puzzle2):
		event = "Puzzle2";
		break;
	case(Puzzle3):
		event = "Puzzle3";
		break;
	}
}
/// <summary>
/// Read the full event and showed on screen
/// </summary>
/// <param name="_eventToRead"></param>
void DialogueManager::ReadDialogue(const eventToRead& _eventToRead) {
	
	if (_writeTextComp->isFinished())
	{
		
		string event;
		ParseEnum(event, _eventToRead);
		if (mRoom[room].find(event) != mRoom[room].end() && !mRoom[room][event].empty()) {

			TextInfo elem = mRoom[room][event].front(); // Gets first element

			_showText->Character = elem.Character; // Saves new text
			_showText->Text = elem.Text;
			cout << elem.Character << ": " << elem.Text << endl;

			_writeTextComp->startTextLine();


			if (_sceneLog) {
				_sceneLog->addDialogueLineLog(elem.Character, elem.Text);
			}

			mRoom[room][event].pop_front(); // Delete read textLine
			
			if (mRoom[room][event].empty())
			{
				//No more dialogue to show
				std::cout << "no more dialogue on event to show" << std::endl;
				_sceneLog->addDialogueLineLog("/", "/");
				//call scene method to dissable dialogue objects on scene

			}
		}
	}
	else
	{
		_writeTextComp->finishTextLine();
	}
	
		
		
		
		
	
}

/// <summary>
/// Show the answer text on screen and upload the value of room
/// </summary>
void DialogueManager::ReadAnswer(){

	//First read the answer, after that upload the value of the room because this means that we finish with the actual room
	//Read and show the answer
	cout << "Ask if a mage do it" << endl;
	++actualroom;
	if (actualroom <= numRooms) {
		room = "Sala" + to_string(actualroom);
	}
}


DialogueManager::~DialogueManager()
{
	delete _showText;
}

void DialogueManager::setSceneLog(LogComponent* sceneLog)
{
	_sceneLog = sceneLog;
}


TextInfo* DialogueManager::getShowText()
{
	return _showText;
}

