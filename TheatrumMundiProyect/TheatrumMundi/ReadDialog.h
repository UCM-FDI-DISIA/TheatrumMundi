#ifndef READDIALOG_H
#define READDIALOG_H

#include <fstream>
#include <cassert>
#include <unordered_map>
#include <deque>

class TextInfo;

using RoomDialogues = std::unordered_map<std::string, std::deque<TextInfo>>;

class ReadDialog {
private:
    std::unordered_map<std::string, RoomDialogues> mRoom;
    int numRooms;

public:
    ReadDialog(int rooms);
    ~ReadDialog();
    void ReadJson();
    RoomDialogues& getRoomDialogues(const std::string& room);
};

#endif
