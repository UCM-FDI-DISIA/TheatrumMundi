
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
	entityManager->setActiveGroup(grp::ZOOMOBJ, false);
}

void SceneRoomTemplate::resolvedPuzzle(int i)
{
	puzzlesol[i] = true;
	entityManager->removeComponent<ClickComponent>(puzzleptr[i]);
	
}

void SceneRoomTemplate::setActiveBottons(bool active)
{
	entityManager->setActiveGroup(grp::UI,active);
}

void SceneRoomTemplate::setActiveZoomObj(bool active)
{
	entityManager->setActiveGroup(grp::ZOOMOBJ, active);
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


