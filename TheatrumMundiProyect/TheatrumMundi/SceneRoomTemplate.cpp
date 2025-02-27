
#include "SceneRoomTemplate.h"
#include "ecs.h"
#include "Manager.h"
#include "DialogueManager.h"
#include "ClickComponent.h"
#include "../src/game/Game.h"
#include <iostream>
using namespace ecs;

void SceneRoomTemplate::startDialogue(const eventToRead& _eventToRead)
{
	entityManager->setActiveGroup(grp::DIALOGUE, true);
	Game::Instance()->getDialogueManager()->ReadDialogue(_eventToRead);

}

void SceneRoomTemplate::endDialogue()
{
	entityManager->setActiveGroup(grp::DIALOGUE, false);
}

void SceneRoomTemplate::resolvedPuzzle(int i)
{
	puzzlesol[i] = true;
	entityManager->removeComponent<ClickComponent>(puzzleptr[i]);
	//si todos los puzzles estan resueltos se llama el evento correspondienteo
}

void SceneRoomTemplate::setActiveBottons(bool active)
{
	entityManager->setActiveGroup(grp::UI,active);
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
	unload();
}


