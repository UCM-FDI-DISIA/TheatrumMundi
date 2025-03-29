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
#include "RectArea2D.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "Image.h"
DragPuzzleScene::DragPuzzleScene() : ScenePuzzleTemplate()
{
	isStarted = false;
    posMat = std::vector(9,std::vector<Vector2D>(8));
   
}

DragPuzzleScene::~DragPuzzleScene()
{

}

void DragPuzzleScene::init()
{
    if (!isStarted) {
        std::list<Area2D*> auxlist;
        //Map tiles
        bool wallMat[8][9] = {
            {1,1,1,1,1,1,1,1,1},
            {1,1,1,1,0,1,1,0,1},
            {1,1,1,0,0,0,0,0,1},
            {1,0,0,0,0,0,0,0,1},
            {1,1,0,0,0,0,0,0,1},
            {1,1,0,0,0,0,0,0,1},
            {1,1,1,0,0,0,0,1,1},
            {1,1,1,1,1,1,1,1,1}
        };
        Vector2D auxposinit = Vector2D(250, 50);
        Vector2D auxpos;
        int auxtiledsize = 64;
        for (int i = 0; i < 9;i++) {
            for (int j = 0;j < 8;j++)
            {
                auxpos = Vector2D(i * auxtiledsize + auxposinit.getX(), j * auxtiledsize + auxposinit.getY());
                if (wallMat[j][i]) {
                    //wall

                    auto a = entityFactory->CreateRectImageWithCollider(entityManager, "MatPared", auxpos, Vector2D(0, 0), auxtiledsize, auxtiledsize, 0, areaLayerManager, ecs::grp::BACKGROUND);
                    auxlist.push_back(a->getMngr()->getComponent<RectArea2D>(a));
                }
                else {
                    //empty space
                    entityFactory->CreateImageEntity(entityManager, "MatHueco", auxpos, Vector2D(0, 0), auxtiledsize, auxtiledsize, 0, ecs::grp::BACKGROUND);
                }
                posMat[i][j] = auxpos;
            }
        }

        //Finish point
        auto goal = entityFactory->CreateTriggerEntity(entityManager, "piezademeta", posMat[7][1], Vector2D(0, 0), auxtiledsize, auxtiledsize, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        entityManager->getComponent<TriggerComponent>(goal)->setTargetGroup(ecs::grp::INTERACTOBJ);
        entityManager->getComponent<TriggerComponent>(goal)->connect(TriggerComponent::AREA_ENTERED, [this] {
            //  std::list<entity_t> collist = goaltrigger->getOverlappingEntities();
            //  for (auto a : collist) {
            Win();
            //  }
            });
        //Drag objCreation;
       //the thing to drag 
        auto thething = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "CosaQueArrastras", posMat[1][3], Vector2D(0, 0), auxtiledsize, auxtiledsize, 1, 1, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(thething));
        /*
         //L UP
         auto _a = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL2",posMat[5][2], Vector2D(0, 0), auxtiledsize * 2, auxtiledsize * 2, 2, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
         entityManager->getComponent<TiledAreaComponent>(_a)->setActiveTile(false, 1, 1);
         auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_a));

         //L DOWM
         auto _b = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL1", posMat[2][5], Vector2D(0, 0), auxtiledsize * 2, auxtiledsize * 2, 2, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
         entityManager->getComponent<TiledAreaComponent>(_b)->setActiveTile(false, 0, 1);
         auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_b));

         // I
         auto _c = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezade3", posMat[4][4], Vector2D(0, 0), auxtiledsize * 1, auxtiledsize * 3, 1, 3, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
         auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_c));

         // 2x2
         auto _d = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezade4", posMat[6][3], Vector2D(0, 0), auxtiledsize * 2, auxtiledsize * 2, 2, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
         auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_d));

         // 2x1
         auto _e = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezade2", posMat[2][3], Vector2D(0, 0), auxtiledsize*2 , auxtiledsize, 2, 1, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
         auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_e));

         //z
         auto _f = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezadez", posMat[5][5], Vector2D(0, 0), auxtiledsize*3, auxtiledsize * 2, 3, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
         auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_f));
         entityManager->getComponent<TiledAreaComponent>(_f)->setActiveTile(false, 0, 0);
         entityManager->getComponent<TiledAreaComponent>(_f)->setActiveTile(false, 2, 1);
         //Add de collisionslist
         entityManager->getComponent<PhysicsBodyComponent>(_b)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_b));
         entityManager->getComponent<PhysicsBodyComponent>(_a)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_a));
         entityManager->getComponent<PhysicsBodyComponent>(_c)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_c));
         entityManager->getComponent<PhysicsBodyComponent>(_d)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_d));
         entityManager->getComponent<PhysicsBodyComponent>(_e)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_e));
         entityManager->getComponent<PhysicsBodyComponent>(_f)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_f));*/
        entityManager->getComponent<PhysicsBodyComponent>(thething)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(thething));
        /*auto a = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL1", Vector2D(0, 0), Vector2D(0, 0), 100, 100, 2, 2, 0, areaLayerManager, ecs::grp::DEFAULT);
        a->getMngr()->getComponent<TiledAreaComponent>(a)->setActiveTile(false,0, 1);
        auto b = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL2", Vector2D(200, 0), Vector2D(0, 0), 100, 100, 2, 2, 0, areaLayerManager, ecs::grp::DEFAULT);
        b->getMngr()->getComponent<TiledAreaComponent>(b)->setActiveTile(false, 1, 1);
        auxlist.push_back(b->getMngr()->getComponent<TiledAreaComponent>(b));
        auxlist.push_back(a->getMngr()->getComponent<TiledAreaComponent>(a));


        a->getMngr()->getComponent<PhysicsBodyComponent>(a)->AddObjectofList(auxlist, a->getMngr()->getComponent<TiledAreaComponent>(a));
        b->getMngr()->getComponent<PhysicsBodyComponent>(b)->AddObjectofList(auxlist, b->getMngr()->getComponent<TiledAreaComponent>(b));*/

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
    std::cout << "WEIN";
}
