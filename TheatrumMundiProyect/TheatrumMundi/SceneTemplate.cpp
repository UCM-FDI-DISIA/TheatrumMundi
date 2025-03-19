#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"
#include "Game.h"
SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	areaLayerManager = new Area2DLayerManager();
	entityFactory = new EntityFactory(entityManager,areaLayerManager);
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
	delete areaLayerManager;
	delete entityFactory;
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

//metodo global inv flechas