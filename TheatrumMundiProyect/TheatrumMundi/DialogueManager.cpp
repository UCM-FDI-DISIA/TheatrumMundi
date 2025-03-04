#include "DialogueManager.h"
#include <fstream>
#include <cassert>
#include "../src/json/json.hpp";
#include "../src/Components/Image.h"
//#include "../src/Components/WriteTextComponent.h";
//#include "TextInfo.h"

#include "../src/components/LogComponent.h"
#include "SceneTemplate.h"


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


DialogueManager::DialogueManager() : _sceneLog(nullptr), _writeTextComp(nullptr), _scene(nullptr), displayOnProcess(false){

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
	case(SalaIntermedia1):
		event = "SalaIntermedia1";
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
		event = "PuzzleLibros";
		break;
	case(Puzzle3):
		event = "PuzzleLibros";
		break;
	case(Calendario):
		event = "Calendario";
		break;
	case(Movil):
		event = "Movil";
		break;
	}
}

/// <summary>
/// Read the first dialogue from the event and showed on screen
/// </summary>
/// <param name="_eventToRead">Id of dialogue event to display</param>
void DialogueManager::ReadDialogue(const eventToRead& _eventToRead) {
	
	string event;
	ParseEnum(event, _eventToRead); //convert id to string
	
	
	if (_writeTextComp->isFinished()) //has dialogueLine finished animating?
	{
		//If dialogueLine has finished, try to display next line

		displayOnProcess = true;

		if (mRoom[room].find(event) != mRoom[room].end() && !mRoom[room][event].empty()) {

			TextInfo elem = mRoom[room][event].front(); // Gets first element

			_showText->Character = elem.Character; // Saves new text
			_showText->Text = elem.Text;

			setCharachterImage(elem.Character);

			_writeTextComp->startTextLine(); //starts animating line

			if (_sceneLog) {
				_sceneLog->addDialogueLineLog(elem.Character, elem.Text); //adds line to log system
			}

			mRoom[room][event].pop_front(); // Delete read textLine
		}
		else
		{
			//Indicate log the dialogue Event has ended
			_sceneLog->addDialogueLineLog("/", "/");
			
			//call scene method to disable dialogue objects on scene
			_scene->endDialogue();
			displayOnProcess = false;

			_showText->Character = " "; // Saves new text
			_showText->Text = " ";
		}
	}
	else
	{
		//Show complete dialogueLine on screen
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

	//UNTOCHABLE
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

void DialogueManager::setScene(SceneTemplate* scene)
{
	_scene = scene;
}

/// <summary>
/// Modify the Texture from the scene
/// </summary>
/// <param name="Character"></param>
void DialogueManager::setCharachterImage(const string& Character)
{
	if (Character == "Keisara") {
		characterimg->setTexture(&sdlutils().images().at("KeisaraSprite"));
	}
	else if (Character == "Lucy") {
		characterimg->setTexture(&sdlutils().images().at("LucySprite"));
	}
	else if (Character == "Sol") {
		characterimg->setTexture(&sdlutils().images().at("SolSprite"));
	}
}

TextInfo* DialogueManager::getShowText()
{
	return _showText;
}

bool DialogueManager::getDisplayOnProcess()
{
	return displayOnProcess;
}

