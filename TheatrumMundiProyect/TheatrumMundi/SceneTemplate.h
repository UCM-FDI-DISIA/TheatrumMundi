#pragma once
#include <list>

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
	void virtual init();
	void virtual update();
	void virtual refresh();
	void virtual unload();
	~SceneTemplate();

};

