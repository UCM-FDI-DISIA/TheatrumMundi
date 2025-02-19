#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"


SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	isStarted = false;
}

void SceneTemplate::update()
{
	entityManager->update();
}

void SceneTemplate::render() const
{
	entityManager->render();
}

SceneTemplate::~SceneTemplate()
{
	unload();
	entityManager->~EntityManager();
	//delete entityManager;
//	delete dialogueManager;
}
