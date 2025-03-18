#pragma once
#include <unordered_map>
#include <iostream>
#include <list>
#include "EventsInfo.h"
#include "TextInfo.h"
#include "../src/Components/WriteTextComponent.h";

template <typename T>
class WriteTextComponent;

class TextInfo;

class Log;

class SceneTemplate;

class Image;

using RoomDialogues = std::unordered_map<std::string,std::list<TextInfo>>; //manage the events and the dialogues
using RoomsMap = std::unordered_map<std::string, RoomDialogues>; //manage the rooms and there dialogues

//A manager which function is to manage all the dialogues of the specific room
class DialogueManager
{
private:
	const int numRooms = 1;
	std::string room;
	int actualroom;
	RoomsMap mRoom; //Map with all the RoomDialogues
	Image* characterimg;
	void ReadJson();
	void ParseEnum(std::string& event, const eventToRead& _eventToRead);

	void setCharachterImage(const std::string& Character);

	TextInfo* _showText; // points to current displayed textLine

	Log* _sceneLog; //points to log list

	WriteTextComponent<TextInfo>* _writeTextComp;

	SceneTemplate* _scene;

	bool displayOnProcess;

public:
	DialogueManager();
	void ReadDialogue(const eventToRead& _eventToRead);
	void ReadAnswer();
	~DialogueManager();
	inline void setCharacterImg(Image* img) {
		characterimg = img;
	}
	void setSceneLog(Log* sceneLog);
	Log* getSceneLog();
	void setScene(SceneTemplate* scene);
	void setWriteTextComp(WriteTextComponent<TextInfo>* writeTextComp)
	{
		_writeTextComp = writeTextComp;
	}
	TextInfo* getShowText();

	bool getDisplayOnProcess();
};

