#include "SceneTemplate.h"
#include "../../TheatrumMundiProyect/src/ecs/ecs.h"
#include "../../TheatrumMundiProyect/src/ecs/Manager.h"
#include "../TheatrumMundi/Area2DLayerManager.h"
#include "DialogueManager.h"
#include "Game.h"

using namespace std;

SceneTemplate::SceneTemplate()
{
	entityManager = new ecs::EntityManager();
	areaLayerManager = new Area2DLayerManager();
	entityFactory = new EntityFactory(entityManager, areaLayerManager);
	dialogueManager = new DialogueManager(0);

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
void SceneTemplate::startDialogue(const string& _eventToRead)
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, true);
	//entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
	dialogueManager->ReadDialogue(_eventToRead);

}

void SceneTemplate::endDialogue()
{
	std::cout << "acabe";
	
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);
}