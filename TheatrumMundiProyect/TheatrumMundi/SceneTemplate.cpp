#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"

SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	entityFactory = new EntityFactory();
	areaLayerManager = new Area2DLayerManager();
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
	
	delete entityManager;
	//delete entityManager;
//	delete dialogueManager;
}
