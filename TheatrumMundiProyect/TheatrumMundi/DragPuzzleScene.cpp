#include "DragPuzzleScene.h"
#include "Vector2D.h"
#include "SDLUtils.h"
#include "DialogueManager.h"
#include "DragComponent.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "TiledAreaComponent.h"
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
        auto a = entityFactory->CreateInteractableEntityTiledCollider(entityManager,"piezaL1",Vector2D(0,0),Vector2D(0,0),100,100,2,2,0,areaLayerManager,ecs::grp::DEFAULT);
        a->getMngr()->getComponent<TiledAreaComponent>(a)->setActiveTile(false,0, 1);
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
