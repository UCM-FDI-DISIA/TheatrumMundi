#pragma once
#include <unordered_map>
#include <iostream>
#include <list>
// Librería que permitre leer los json a partir de cierto punto #include <nlohmann/json.hpp>
//A struct with all the dialogue information
struct TextInfo {
	std::string Character;
	std::string Text;
};
enum eventToRead {
	SalaIntermediaEvento1,
	SalaIntermediaEvento2,
	SalaIntermediaEvento3,
	Pista1,
	Pista2,
	Pista3,
	Puzzle1,
	Puzzle2,
	Puzzle3
};
using RoomDialogues = std::unordered_map<std::string,std::list<TextInfo>>; //manage the events and the dialogues
using RoomsMap = std::unordered_map<std::string, RoomDialogues>; //manage the rooms and there dialogues
//A manager which function is to manage all the dialogues of the specific room
class DialogueManager
{
private:
	const int numRooms = 3;
	std::string room;
	int actualroom;
	RoomsMap mRoom; //Map with all the RoomDialogues
	void ReadJson();
	void ParseEnum(std::string& event, const eventToRead& _eventToRead);
public:
	DialogueManager();
	void ReadDialogue(const eventToRead& _eventToRead); 
	void ReadAnswer();
};

