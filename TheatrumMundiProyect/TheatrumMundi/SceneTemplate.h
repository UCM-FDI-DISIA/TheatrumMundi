#pragma once
#include <list>

#include "../../TheatrumMundiProyect/src/ecs/Manager.h"

#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"


class SceneTemplate
{
protected:
	 ecs::EntityManager* entityManager;
	 EntityFactory* entityFactory;
	// DialogueManager* dialogueManager;
	 bool isStarted;
public:
	SceneTemplate();
	void virtual init() {};
	void virtual init(ecs::entity_t& ent) {};
	void update();
	void render() const;
	void virtual refresh() {};
	void virtual unload() {};
	virtual ~SceneTemplate();

};

