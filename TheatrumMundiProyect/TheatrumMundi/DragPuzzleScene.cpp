#include "DragPuzzleScene.h"
#include "Vector2D.h"
#include "DialogueManager.h"
DragPuzzleScene::DragPuzzleScene() : ScenePuzzleTemplate()
{

}

DragPuzzleScene::~DragPuzzleScene()
{

}

void DragPuzzleScene::init(SceneRoomTemplate* sr)
{
    if (!isStarted) {


        dialogueManager->Init(2,entityFactory,entityManager,false,areaLayerManager,"arrastracosaspuzzle");
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
