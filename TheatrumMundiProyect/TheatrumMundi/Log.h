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

	std::list<TextInfo> _renderedDialogueLines; //list of current dialogue lines displayed on log menu
	std::list<TextInfo>::iterator _firstRenderLine; //points to current first dialogue line to be displayed on log menu

	const int LINES_DISPLAYED = 5; //number of lines displayed on screen

	SceneTemplate* sceneTemplate;

public:
	
	Log();

	//add new dialogue line to log registry
	void addDialogueLineLog(std::string author, std::string dialogueLine);
	void cleanLogList();
	void cleanRenderedList();

	void SetLogActive(bool logActive);
	bool GetLogActive();

	void Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager, SceneTemplate* scTp);

	//delete of enitities
	~Log();

	std::list<TextInfo>* getLogList() {	return &_log;} //returns log list

	std::list<TextInfo>* getRenderedDialogueLines() { return &_renderedDialogueLines; }

	void setRenderedDialogueLines(); //sets rendering dialogue lines dependig of iterator

	void next(); //used for scroll

	void previous(); //used for scroll

	void ResetLog();
};