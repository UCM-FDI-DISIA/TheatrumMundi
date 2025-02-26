#include "SceneRoomTemplate.h"
#include "ecs.h"
#include "Manager.h"
#include "DialogueManager.h"
#include "ClickComponent.h"
#include <iostream>
using namespace ecs;
/*void SceneRoomTemplate::startDialogue(const DialogueManager)
{
	entityManager->setActiveGroup(grp::DIALOGUE,true);
	//que co�o se hace aqui
//m->ReadDialogue(event);
	dm->ReadDialogue(_eventToRead);

}
}*/

void SceneRoomTemplate::endDialogue()
{
	entityManager->setActiveGroup(grp::DIALOGUE, false);
}

void SceneRoomTemplate::resolvedPuzzle(int i)
{
	puzzlesol[i] = true;
	entityManager->setActive(puzzleptr[i], false);
	entityManager->removeComponent<ClickComponent>(puzzleptr[i]);
	//si todos los puzzles estan resueltos se llama el evento correspondienteo
}

void SceneRoomTemplate::setActiveBottons(bool active)
{
	entityManager->setActiveGroup(grp::UI, active);
}

void SceneRoomTemplate::setActiveInteractObj(bool active)
{
	entityManager->setActiveGroup(grp::INTERACTOBJ, active);
}

SceneRoomTemplate::SceneRoomTemplate() : SceneTemplate()
{
	for (int i = 0; i < 3;i++)puzzlesol.push_back(false);
	//init inventory
}

SceneRoomTemplate::~SceneRoomTemplate()
{
	delete dm;
	unload();
}


