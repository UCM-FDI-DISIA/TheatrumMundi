#pragma once
#include <unordered_map>
#include <iostream>
#include <list>


template <typename T>
class WriteTextComponent;

class TextInfo;

class LogComponent;

//A struct with all the dialogue information
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
	
	WriteTextComponent<TextInfo>* WriteText;

	LogComponent* _sceneLog;
public:
	DialogueManager();
	void ReadDialogue(const eventToRead& _eventToRead);
	void ReadAnswer();
	~DialogueManager();

	void setSceneLog(LogComponent* sceneLog);
	void setWriteText(WriteTextComponent<TextInfo>* textoToWrite);
};

