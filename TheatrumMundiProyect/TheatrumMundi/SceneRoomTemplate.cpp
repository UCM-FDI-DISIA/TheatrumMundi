
#include "SceneRoomTemplate.h"
#include "ecs.h"
#include "Manager.h"
#include "DialogueManager.h"
#include "ClickComponent.h"
#include "../src/game/Game.h"
#include <iostream>
using namespace ecs;

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
	inv = new Inventory();
}

SceneRoomTemplate::~SceneRoomTemplate()
{
	unload();
}


