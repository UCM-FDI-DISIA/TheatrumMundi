#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"
#include "Game.h"
#include "Log.h"

SceneTemplate::SceneTemplate()
{
	inv = new Inventory();
	entityManager = new ecs::EntityManager();
	areaLayerManager = new Area2DLayerManager();
	entityFactory = new EntityFactory(entityManager,areaLayerManager);
	isStarted = false;
	//sceneLog = new Log();
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
	delete areaLayerManager;
	delete entityFactory;
	delete sceneLog;
}
void SceneTemplate::startDialogue(const eventToRead& _eventToRead)
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, true);
	//entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
	Game::Instance()->getDialogueManager()->ReadDialogue(_eventToRead);

}

void SceneTemplate::endDialogue()
{
	std::cout << "acabe";
	
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);
}