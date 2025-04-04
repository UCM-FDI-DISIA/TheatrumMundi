
#include "SceneRoomTemplate.h"
#include "ecs.h"
#include "Manager.h"
#include "DialogueManager.h"
#include "ClickComponent.h"
#include "../src/game/Game.h"
#include <iostream>
using namespace ecs;




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

void SceneRoomTemplate::scrollInventory(int dir)
{
    if (!GetInventory() || GetInventory()->hints.empty()) return;

    if (dir == -1) { // Scroll UP
        if (GetInventory()->getFirstItem() > 0) {

            int lastVisibleIndex = GetInventory()->getFirstItem() + GetInventory()->getItemNumber() - 1;
            if (lastVisibleIndex < GetInventory()->hints.size()) {
                GetInventory()->hints[lastVisibleIndex]->getMngr()->setActive(GetInventory()->hints[lastVisibleIndex], false);
            }


            GetInventory()->setFirstItem(GetInventory()->getFirstItem() - 1);


            GetInventory()->hints[GetInventory()->getFirstItem()]->getMngr()->setActive(GetInventory()->hints[GetInventory()->getFirstItem()], true);


            for (auto& hint : GetInventory()->hints) {
                auto transform = hint->getMngr()->getComponent<Transform>(hint);
                transform->setPosY(transform->getPos().getY() + 150);
            }
        }
    }
    else if (dir == 1) { // Scroll DOWN
		if (GetInventory()->getFirstItem() + GetInventory()->getItemNumber() < GetInventory()->hints.size()) //take care of the case when we are at the end of the inventory
        {
            GetInventory()->hints[GetInventory()->getFirstItem()]->getMngr()->setActive(GetInventory()->hints[GetInventory()->getFirstItem()], false);
            GetInventory()->setFirstItem(GetInventory()->getFirstItem() + 1);

            int newLastVisibleIndex = GetInventory()->getFirstItem() + GetInventory()->getItemNumber() - 1;
            if (newLastVisibleIndex < GetInventory()->hints.size()) {
                GetInventory()->hints[newLastVisibleIndex]->getMngr()->setActive(GetInventory()->hints[newLastVisibleIndex], true);
            }


            for (auto& hint : GetInventory()->hints) {
                auto transform = hint->getMngr()->getComponent<Transform>(hint);
                transform->setPosY(transform->getPos().getY() - 150);
            }
        }
    }
}

void SceneRoomTemplate::reposInvRoom(SceneRoomTemplate* sr)
{
    for (int i = 0; i < GetInventory()->hints.size(); ++i) {
        GetInventory()->hints[i]->getMngr()->getComponent<Transform>(GetInventory()->hints[i])->getPos().set(GetInventory()->GetPosition(i));
    }
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


