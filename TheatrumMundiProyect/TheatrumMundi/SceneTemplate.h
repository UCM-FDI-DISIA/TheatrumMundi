#pragma once
#include <list>
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
class EntityManager {

};
class DialogueManager {};

class SceneTemplate
{
private:
	 EntityManager* entityManager;
	 DialogueManager* dialogueManager;
public:
	SceneTemplate();
	void virtual init() {};
	void virtual init(const ecs::entity_t& ent) {};
	void virtual update() {
	};
	void virtual refresh() {};
	void virtual unload() {};
	~SceneTemplate();

};

