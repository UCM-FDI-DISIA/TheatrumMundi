#pragma once
#include <unordered_map>
#include <iostream>
#include <list>
// Librería que permitre leer los json a partir de cierto punto #include <nlohmann/json.hpp>
//A struct with all the dialogue information
struct TextInfo {
	std::string Characther;
	std::string Text;
};
using RoomDialogues = std::unordered_map<std::string,std::list<TextInfo>>; 
using AnswerDialogues = std::unordered_map<std::string, std::string>;
//A manager which function is to manage all the dialogues of the specific room
class DialogueManager
{
private:
	RoomDialogues mRoomDialogues; //Map with all the RoomDialogues
	AnswerDialogues  mAnswerDialogues; //Map with all the possible answers 
	void ReadJson();
public:
	DialogueManager(); 
	void ReadDialogue(); 
};

