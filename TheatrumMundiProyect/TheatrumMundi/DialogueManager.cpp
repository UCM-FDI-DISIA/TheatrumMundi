#include "DialogueManager.h"
#include <fstream>
#include <cassert>
#include "../src/json/json.hpp";

using json = nlohmann::json;

using namespace std;

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
		for (auto& elem : dialogues[room].items()) {
			for (auto& elem2 : elem.value()) {
				string character = to_string(elem2["Character"]);
				string text = to_string(elem2["Text"]);
				r[elem.key()].push_back(TextInfo{ character,text });
			}
		}
		mRoom[room] = r;
	}

}

DialogueManager::DialogueManager(){
	ReadJson();
}

void DialogueManager::ReadDialogue()
{

}
