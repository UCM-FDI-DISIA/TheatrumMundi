#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"
#include "DialogueManager.h"
#include "Area2D.h"
#include "PauseManager.h"
#include "DialogueAnimComponent.h";
#include "Game.h"

using namespace std;

SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	areaLayerManager = new Area2DLayerManager();
	entityFactory = new EntityFactory(entityManager, areaLayerManager);
	pauseManager = new PauseManager();
	dialogueManager = nullptr;
	logbtn = nullptr;
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

void SceneTemplate::refresh()
{
	entityManager->refresh();
}

SceneTemplate::~SceneTemplate()
{
	unload();
	
	delete pauseManager;
	delete entityFactory;
	if(dialogueManager)delete dialogueManager;
	delete entityManager;
	delete areaLayerManager;
}
void SceneTemplate::startDialogue(const string& _eventToRead)
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, true);
	for (auto a : entityManager->getEntities(ecs::grp::DIALOGUE)) {
		if(a->getMngr()->getComponent<DialogueAnimComponent>(a) != nullptr) 
		a->getMngr()->getComponent<DialogueAnimComponent>(a)->startDialogueAnim();
	}
	dialogueManager->setdisplayOnProcess(true);
	//entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
	dialogueManager->setEventToRead(_eventToRead);
	dialogueManager->ReadDialogue(_eventToRead);

}

void SceneTemplate::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);
	
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);

}

void SceneTemplate::closedMenus()
{
	closedLog();
	if(logbtn != nullptr) entityManager->setActive(logbtn, true);
}

//metodo global inv flechas