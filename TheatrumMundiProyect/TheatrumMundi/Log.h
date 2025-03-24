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


class Log
{
private:
	// a list of pairs to save all showed dialogue lines
	// first: author
	// second: dialogueLine
	std::list<TextInfo> _log;
	bool _logActive;

public:
	
	Log();

	//add new dialogue line to log registry
	void addDialogueLineLog(std::string author, std::string dialogueLine);

	void SetLogActive(bool logActive);
	bool GetLogActive();

	void Init(EntityFactory* entityFactory, ecs::EntityManager* entityManager, Area2DLayerManager* areaLayerManager);



	//delete of enitities
	~Log();

	std::list<TextInfo>* getLogList()
	{
		return &_log;
	}



};