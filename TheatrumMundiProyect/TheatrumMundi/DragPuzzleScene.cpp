#include "DragPuzzleScene.h"
#include "Vector2D.h"
#include "SDLUtils.h"
#include "DialogueManager.h"
#include "DragComponent.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "TiledAreaComponent.h"
#include <list>
#include "Area2D.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "Image.h"
DragPuzzleScene::DragPuzzleScene() : ScenePuzzleTemplate()
{
	isStarted = false;
}

DragPuzzleScene::~DragPuzzleScene()
{

}

void DragPuzzleScene::init()
{
    if (!isStarted) {
        std::list<Area2D*> auxlist;
        auto a = entityFactory->CreateInteractableEntityTiledCollider(entityManager,"piezaL1",Vector2D(0,0),Vector2D(0,0),100,100,2,2,0,areaLayerManager,ecs::grp::DEFAULT);
        a->getMngr()->getComponent<TiledAreaComponent>(a)->setActiveTile(false,0, 1);
        auto b = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL2", Vector2D(200, 0), Vector2D(0, 0), 100, 100, 2, 2, 0, areaLayerManager, ecs::grp::DEFAULT);
        b->getMngr()->getComponent<TiledAreaComponent>(b)->setActiveTile(false, 1, 1);
        auxlist.push_back(b->getMngr()->getComponent<TiledAreaComponent>(b));
        auxlist.push_back(a->getMngr()->getComponent<TiledAreaComponent>(a));


        a->getMngr()->getComponent<PhysicsBodyComponent>(a)->AddObjectofList(auxlist, a->getMngr()->getComponent<TiledAreaComponent>(a));
        b->getMngr()->getComponent<PhysicsBodyComponent>(b)->AddObjectofList(auxlist, b->getMngr()->getComponent<TiledAreaComponent>(b));
    }
}

void DragPuzzleScene::unload()
{
}

void DragPuzzleScene::Exit()
{
}

bool DragPuzzleScene::Check()
{
    if (_goalPos ==_triggerObj->getPos()) {
        Win();
        return true;
   }
    return false;
}

void DragPuzzleScene::Win()
{

}
