#include "ReadDialog.h"
#include "TextInfo.h"
#include "../src/json/json.hpp"  // Incluye json.hpp solo aquí

using json = nlohmann::json;  // Alias para json

using namespace std;

ReadDialog::ReadDialog(int rooms) : numRooms(rooms) {
    ReadJson();
}

void ReadDialog::ReadJson() {
	//Open de json path

	ifstream mJson("../resources/config/dialogues.json");
	assert(mJson);
	json dialogues;
	mJson >> dialogues;

	//set room dialogues
	for (int i = 0; i <= numRooms; ++i) {

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

RoomDialogues& ReadDialog::getRoomDialogues(const string& room) {
    return mRoom[room];
}
