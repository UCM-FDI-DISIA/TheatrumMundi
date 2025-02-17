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
			cout << elem.value()[0];
			//string character = to_string(elem.value()[0]);
			//string text = elem["Text"].get<string>();
			r[elem.key()].emplace_back(TextInfo{ "character","text" });
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
