#pragma once

#include <list>
#include <string>
#include "TextInfo.h"
#include "ecs.h"

class SceneTemplate;
class EntityFactory;
class Image;
class Area2DLayerManager;
class TextInfo;
class ClickComponent;

class Log
{
private:
	std::list<TextInfo> _log; // complete log list: contains all already displayed dialogue lines
	bool _logActive; //checks if log menu is active
	ClickComponent* _textDialogueComp; //needed for log scroll

	std::list<TextInfo> _renderedDialogueLines; //list of current dialogue lines displayed on log menu
	std::list<TextInfo>::iterator _firstRenderLine; //points to current first dialogue line to be displayed on log menu

	const int LINES_DISPLAYED = 5; //number of lines displayed on screen

public:
	
	Log();

	//add new dialogue line to log registry
	void addDialogueLineLog(std::string author, std::string dialogueLine);
	void cleanLogList();
	void cleanRenderedList();

	void SetLogActive(bool logActive);
	bool GetLogActive();

	void Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager);

	//delete of enitities
	~Log();

	std::list<TextInfo>* getLogList() {	return &_log;} //returns log list

	void setTextDialogue(ClickComponent* textDialogue) { _textDialogueComp = textDialogue;}

	std::list<TextInfo>* getRenderedDialogueLines() { return &_renderedDialogueLines; }

	void setRenderedDialogueLines(); //sets rendering dialogue lines dependig of iterator

	void next() { std::advance(_firstRenderLine, std::min(5, static_cast<int>(std::distance(_firstRenderLine, _log.end())))); } //used for scroll

	void previous() {std::advance(_firstRenderLine, -std::min(5, static_cast<int>(std::distance(_log.begin(), _firstRenderLine))));} //used for scroll
};