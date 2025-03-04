#pragma once
#include <list>
#include "EventsInfo.h"
#include "Manager.h"

#include "../../TheatrumMundiProyect/src/ecs/Manager.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"

class Area2DLayerManager;
class SceneRoomTemplate;

class SceneTemplate
{
protected:
	 ecs::EntityManager* entityManager;
	 EntityFactory* entityFactory;
	// DialogueManager* dialogueManager;
	 Area2DLayerManager* areaLayerManager;
	 
	bool isStarted;
public:
	SceneTemplate();
	void virtual init() {};
	void virtual init(SceneRoomTemplate* s) {};
	void update();
	void render() const;
	void virtual refresh() {};
	void virtual unload() {};
	virtual ~SceneTemplate();

	void startDialogue(const eventToRead& _eventToRead);

	void endDialogue();

};

