#pragma once
#include <list>

#include "../../TheatrumMundiProyect/src/ecs/Manager.h"




class SceneTemplate
{
protected:
	 ecs::EntityManager* entityManager;
	// DialogueManager* dialogueManager;
public:
	SceneTemplate();
	void virtual init() {};
	void virtual init(ecs::entity_t& ent) {};
	void update();
	void render() const;
	void virtual refresh() {};
	void virtual unload() {};
	~SceneTemplate();

};

