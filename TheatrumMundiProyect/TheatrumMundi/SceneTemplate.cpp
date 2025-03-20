#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"
#include "DialogueManager.h"
#include "Game.h"
#include "Log.h"

using namespace std;

SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	areaLayerManager = new Area2DLayerManager();
	entityFactory = new EntityFactory(entityManager, areaLayerManager);
	dialogueManager = new DialogueManager(0);

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
void SceneTemplate::startDialogue(const string& _eventToRead)
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, true);
	dialogueManager->setdisplayOnProcess(true);
	//entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
	dialogueManager->ReadDialogue(_eventToRead);

}

void SceneTemplate::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);
	std::cout << "acabe";
	
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);
}