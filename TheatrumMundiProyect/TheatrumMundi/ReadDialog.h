#ifndef READDIALOG_H
#define READDIALOG_H

#include <iostream>
#include <fstream>
#include <cassert>
#include <unordered_map>
#include <deque>
#include "../src/json/json.hpp"

using json = nlohmann::json;
using namespace std;

struct TextInfo {
    string Character;
    string Text;
};

using RoomDialogues = unordered_map<string, deque<TextInfo>>;

class ReadDialog {
private:
    unordered_map<string, RoomDialogues> mRoom;
    int numRooms;

public:
    ReadDialog(int rooms);
    void ReadJson();
    RoomDialogues& getRoomDialogues(const string& room);
};

#endif
