#include "ReadDialog.h"
#include "TextInfo.h"
#include "../src/json/json.hpp"  // Incluye json.hpp solo aqu�

using json = nlohmann::json;  // Alias para json

using namespace std;

ReadDialog::ReadDialog(int rooms) : numRooms(rooms) {
    ReadJson();
}

ReadDialog::~ReadDialog()
{

}


void ReadDialog::ReadJson() {
	//Open de json path

	ifstream mJson("../resources/config/dialogues.json");
	assert(mJson);
	json dialogues;
	mJson >> dialogues;

	//set room dialogues
	for (int i = -1; i <= numRooms; ++i) {

		string room = "Sala" + to_string(i);
		cout << "cout room tutorial: " <<  room << endl;
		assert(dialogues.contains(room));
		RoomDialogues r;

		//Set all events from the specific room
		for (auto& elem : dialogues[room].items()) {

			//Fill r with all the evenet dialogues 
			for (auto& elem2 : elem.value()) {

				string character = elem2["Character"];
				string text = elem2["Text"];
				r[elem.key()].emplace_back(TextInfo{ character, text });
			}
		}

		//Upload Map
		mRoom[room] = r;
	}
	dialogues.clear();  // Vac�a su contenido
	dialogues = json();    // Libera la memoria
	mJson.close();
}

void ReadDialog::ResetReader()
{
	ReadJson();
}

RoomDialogues& ReadDialog::getRoomDialogues(const string& room) {
    return mRoom[room];
}
