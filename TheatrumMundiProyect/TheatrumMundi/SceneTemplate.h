#pragma once
#include <list>

#include "Manager.h"

#include "../../TheatrumMundiProyect/src/ecs/Manager.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"

class Area2DLayerManager;

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
	void virtual init(SceneTemplate* s) {};
	void update();
	void render() const;
	void virtual refresh() {};
	void virtual unload() {};
	virtual ~SceneTemplate();

};

