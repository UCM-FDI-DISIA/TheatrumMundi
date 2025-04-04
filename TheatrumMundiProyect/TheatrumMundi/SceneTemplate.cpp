#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"
#include "DialogueManager.h"
#include "PauseManager.h"
#include "Game.h"

using namespace std;

SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	areaLayerManager = new Area2DLayerManager();
	entityFactory = new EntityFactory(entityManager, areaLayerManager);
	dialogueManager = new DialogueManager(0);
	pauseManager = new PauseManager();

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
	delete dialogueManager;
}
void SceneTemplate::startDialogue(const string& _eventToRead)
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, true);
	dialogueManager->setdisplayOnProcess(true);
	//entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
	dialogueManager->setEventToRead(_eventToRead);
	dialogueManager->ReadDialogue(_eventToRead);

}

void SceneTemplate::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);

}

//metodo global inv flechas