#include "ReadDialog.h"

ReadDialog::ReadDialog(int rooms) : numRooms(rooms) {
    ReadJson();
}

void ReadDialog::ReadJson() {
    ifstream mJson("../resources/config/dialogues.json");
    assert(mJson);
    json dialogues;
    mJson >> dialogues;

    for (int i = 1; i <= numRooms; ++i) {
        string room = "Sala" + to_string(i);
        assert(dialogues.contains(room));
        RoomDialogues r;

        for (auto& elem : dialogues[room].items()) {
            for (auto& elem2 : elem.value()) {
                string character = elem2["Character"];
                string text = elem2["Text"];
                r[elem.key()].push_back(TextInfo{ character, text });
            }
        }
        mRoom[room] = r;
    }
}

RoomDialogues& ReadDialog::getRoomDialogues(const string& room) {
    return mRoom[room];
}
