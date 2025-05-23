
#include "SceneRoomTemplate.h"
#include "ecs.h"
#include "Manager.h"
#include "DialogueManager.h"
#include "ClickComponent.h"
#include "Transform.h"
#include "TriggerComponent.h"
#include "Area2DLayerManager.h"
#include "RectArea2D.h"
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
            areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(GetInventory()->hints[GetInventory()->getFirstItem()])->getLayerPos());


            for (auto& hint : GetInventory()->hints) {
                auto transform = hint->getMngr()->getComponent<Transform>(hint);
                Vector2D auxVector = Vector2D(transform->getPos().getX(), transform->getPos().getY() + (150 * Game::Instance()->hscreenScale));
                transform->setPosPure(auxVector);
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
                areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(GetInventory()->hints[newLastVisibleIndex])->getLayerPos());
            }


            for (auto& hint : GetInventory()->hints) {
                auto transform = hint->getMngr()->getComponent<Transform>(hint);
                Vector2D auxVector = Vector2D(transform->getPos().getX(), transform->getPos().getY() - (150 * Game::Instance()->hscreenScale));
                transform->setPosPure(auxVector);
            }
        }
    }
}


/// <summary>
/// Hide the invnentory when a scene is called
/// </summary>
/// <param name="invBack"></param>
/// <param name="UpButton"></param>
/// <param name="DownButt"></param>
void SceneRoomTemplate::HideAllInvetoryItems(const ecs::entity_t& invBack, const ecs::entity_t& UpButton, const ecs::entity_t& DownButt)
{
    GetInventory()->setActive(false);
    invBack->getMngr()->setActive(invBack, false);
    UpButton->getMngr()->setActive(UpButton, false);
    DownButt->getMngr()->setActive(DownButt, false);
    for (int i = 0; i < GetInventory()->getItems().size(); ++i)
        GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Desactivate the hints
}

void SceneRoomTemplate::createDescription(ecs::entity_t hint, Hint* desc)
{
    //if mouse is on item, show item description
   hint->getMngr()->getComponent<TriggerComponent>(hint)->connect(TriggerComponent::CURSOR_ENTERED, [this, hint,desc]() {
        //show item description entities
        entityManager->setActive(invObjects.textDescriptionEnt, true);

        //change text description
        inv->setTextDescription(desc, entityManager->getComponent<Transform>(hint));
        });

    //if mouse leaves item, hide item description
    hint->getMngr()->getComponent<TriggerComponent>(hint)->connect(TriggerComponent::CURSOR_LEFT, [this]() {
        //hide item description entities
        entityManager->setActive(invObjects.textDescriptionEnt, false);
        });

    
}

void SceneRoomTemplate::reposAllInventoryItems()
{
    for (int i = 0; i < GetInventory()->hints.size(); ++i) {
        GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);
        GetInventory()->hints[i]->getMngr()->getComponent<Transform>(GetInventory()->hints[i])->setPos(GetInventory()->GetPosition(i));
    }
}

void SceneRoomTemplate::unload()
{
    
}

SceneRoomTemplate::SceneRoomTemplate() : SceneTemplate()
{
	for (int i = 0; i < 3;i++)puzzlesol.push_back(false);
	inv = new Inventory();
}

SceneRoomTemplate::~SceneRoomTemplate()
{
	unload();
    delete inv;
}


