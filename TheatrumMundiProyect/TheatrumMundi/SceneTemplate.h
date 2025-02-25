#pragma once
#include <list>

#include "Manager.h"
class Area2DLayerManager;

class SceneTemplate
{
protected:
	 ecs::EntityManager* entityManager;
	// DialogueManager* dialogueManager;
	 Area2DLayerManager* areaLayerManager;
	 
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

