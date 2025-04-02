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
    auxtiledsize = Vector2D(64, 64);
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
        Vector2D auxposinit = Vector2D(256, 64);
        Vector2D auxpos;

        for (int i = 0; i < 9;i++) {
            for (int j = 0;j < 8;j++)
            {
                entity_t a;
                auxpos = Vector2D(i * auxtiledsize.getX() + auxposinit.getX() * Game::Instance()->wscreenScale, j * auxtiledsize.getY() + auxposinit.getY() * Game::Instance()->wscreenScale);
                if (wallMat[j][i]) {
                    //wall

                    a = entityFactory->CreateRectImageWithCollider(entityManager, "MatPared", auxpos, Vector2D(0, 0), auxtiledsize.getX(), auxtiledsize.getY(), 0, areaLayerManager, ecs::grp::BACKGROUND);
                    auxlist.push_back(a->getMngr()->getComponent<RectArea2D>(a));
                }
                else {
                    //empty space
                    a =entityFactory->CreateImageEntity(entityManager, "MatHueco", auxpos, Vector2D(0, 0), auxtiledsize.getX(), auxtiledsize.getY(), 0, ecs::grp::BACKGROUND);
                }
                posMat[i][j] = entityManager->getComponent<Transform>(a)->getPos();
            }
        }
        //Drag objCreation;
      //the thing to drag 
        _triggerObj = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "CosaQueArrastras", posMat[1][3], Vector2D(0, 0), auxtiledsize.getX(), auxtiledsize.getY(), 1, 1, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_triggerObj));
        entityManager->getComponent<DragComponent>(_triggerObj)->connect(DragComponent::DRAG_END, [this]() {
            Transform* aux = entityManager->getComponent<Transform>(_triggerObj);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });
        //Finish point
        auto goal = entityFactory->CreateTriggerEntity(entityManager, "piezademeta", posMat[7][1], Vector2D(0, 0), auxtiledsize.getX(), auxtiledsize.getY(), 0, areaLayerManager, ecs::grp::BACKGROUND);
        TriggerComponent* goaltrigger = entityManager->getComponent<TriggerComponent>(goal);
        entityManager->getComponent<Transform>(goal)->setPosPure(posMat[7][1]);
        goaltrigger->setTargetGroup(ecs::grp::INTERACTOBJ);
        goaltrigger->connect(TriggerComponent::AREA_ENTERED, [this, goaltrigger] {

            std::list<entity_t> collist = goaltrigger->getOverlappingEntities();
            for (auto a : collist) {
                if (a == _triggerObj)Check();
            }
            });


        //L UP
        auto _a = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL2", posMat[5][2], Vector2D(0, 0), auxtiledsize.getX() * 2, auxtiledsize.getY() * 2, 2, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        entityManager->getComponent<TiledAreaComponent>(_a)->setActiveTile(false, 1, 1);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_a));
        entityManager->getComponent<DragComponent>(_a)->connect(DragComponent::DRAG_END, [this, _a]() {
            Transform* aux = entityManager->getComponent<Transform>(_a);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });

        //L DOWM
        auto _b = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezaL1", posMat[2][5], Vector2D(0, 0), auxtiledsize.getX() * 2, auxtiledsize.getY() * 2, 2, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        entityManager->getComponent<TiledAreaComponent>(_b)->setActiveTile(false, 0, 1);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_b));
        entityManager->getComponent<DragComponent>(_b)->connect(DragComponent::DRAG_END, [this, _b]() {
            Transform* aux = entityManager->getComponent<Transform>(_b);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });

        // I
        auto _c = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezade3", posMat[4][4], Vector2D(0, 0), auxtiledsize.getX() * 1, auxtiledsize.getY() * 3, 1, 3, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_c));
        entityManager->getComponent<DragComponent>(_c)->connect(DragComponent::DRAG_END, [this, _c]() {
            Transform* aux = entityManager->getComponent<Transform>(_c);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });
        // 2x2
        auto _d = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezade4", posMat[6][3], Vector2D(0, 0), auxtiledsize.getX() * 2, auxtiledsize.getY() * 2, 2, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_d));
        entityManager->getComponent<DragComponent>(_d)->connect(DragComponent::DRAG_END, [this, _d]() {
            Transform* aux = entityManager->getComponent<Transform>(_d);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });
        // 2x1
        auto _e = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezade2", posMat[2][3], Vector2D(0, 0), auxtiledsize.getX() * 2, auxtiledsize.getY(), 2, 1, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_e));
        entityManager->getComponent<DragComponent>(_e)->connect(DragComponent::DRAG_END, [this, _e]() {
            Transform* aux = entityManager->getComponent<Transform>(_e);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });
        //z
        auto _f = entityFactory->CreateInteractableEntityTiledCollider(entityManager, "piezadez", posMat[5][5], Vector2D(0, 0), auxtiledsize.getX() * 3, auxtiledsize.getY() * 2, 3, 2, 0, areaLayerManager, ecs::grp::INTERACTOBJ);
        auxlist.push_back(entityManager->getComponent<TiledAreaComponent>(_f));
        entityManager->getComponent<TiledAreaComponent>(_f)->setActiveTile(false, 0, 0);
        entityManager->getComponent<TiledAreaComponent>(_f)->setActiveTile(false, 2, 1);
        entityManager->getComponent<DragComponent>(_f)->connect(DragComponent::DRAG_END, [this, _f]() {
            Transform* aux = entityManager->getComponent<Transform>(_f);
            aux->setPosPure(NearMatPoint(aux->getPos()));
            });
        //Add de collisionslist
        //entityManager->getComponent<PhysicsBodyComponent>(_b)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_b));
       
       // entityManager->getComponent<PhysicsBodyComponent>(_c)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_c));
        //entityManager->getComponent<PhysicsBodyComponent>(_d)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_d));
       // entityManager->getComponent<PhysicsBodyComponent>(_e)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_e));
       entityManager->getComponent<PhysicsBodyComponent>(_a)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_a));
        entityManager->getComponent<PhysicsBodyComponent>(_f)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_f));
        entityManager->getComponent<PhysicsBodyComponent>(_triggerObj)->AddObjectofList(auxlist, entityManager->getComponent<TiledAreaComponent>(_triggerObj));

        Transform* _atr = entityManager->getComponent<Transform>(_a);
        Transform* _btr = entityManager->getComponent<Transform>(_b);
        Transform* _ctr = entityManager->getComponent<Transform>(_c);
        Transform* _dtr = entityManager->getComponent<Transform>(_d);
        Transform* _etr = entityManager->getComponent<Transform>(_e);
        Transform* _ftr = entityManager->getComponent<Transform>(_f);
        Transform* _triggerobjtr = entityManager->getComponent<Transform>(_triggerObj);
        //Reset Btn
        auto _reset = entityFactory->CreateInteractableEntity(entityManager, "Hanni", EntityFactory::RECTAREA, posMat[0][0], Vector2D(0, 0), auxtiledsize.getX(), auxtiledsize.getY(), 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
        entityManager->getComponent<ClickComponent>(_reset)->connect(ClickComponent::JUST_CLICKED, [this, _atr, _btr, _ctr, _dtr, _etr, _ftr, _triggerobjtr]() {
            if (!solved) {
                _atr->setPosPure(posMat[5][2]);
                _btr->setPosPure(posMat[2][5]);
                _ctr->setPosPure(posMat[4][4]);
                _dtr->setPosPure(posMat[6][3]);
                _etr->setPosPure(posMat[2][3]);
                _ftr->setPosPure(posMat[5][5]);
                _triggerobjtr->setPosPure(posMat[1][3]);
            }
            });

        //RElative position in posmat
        _atr->setPosPure(posMat[4][2]);
        _btr->setPosPure(posMat[2][5]);
        _ctr->setPosPure(posMat[4][4]);
        _dtr->setPosPure(posMat[6][3]);
        _etr->setPosPure(posMat[2][3]);
        _ftr->setPosPure(posMat[5][5]);
        _triggerobjtr->setPosPure(posMat[1][3]);
       
        auxtiledsize.setX(auxtiledsize.getX()*Game::Instance()->wscreenScale);
         auxtiledsize.setY(auxtiledsize.getY()*Game::Instance()->hscreenScale);
    }
}

void DragPuzzleScene::unload()
{
}

Vector2D DragPuzzleScene::NearMatPoint(Vector2D pos)
{
    int auxi = 0;
    int auxj = 0;
    bool _aux = false;
    while (!_aux){
        if  (pos.getX()- posMat[auxi][auxj].getX() < auxtiledsize.getX()) {
            if (abs(posMat[auxi][auxj].getX() - pos.getX()) >abs(posMat[auxi + 1][auxj].getX() - pos.getX())) {
                auxi++;
                _aux = true;
            }
            else {
                _aux = true;
            }
        }
        else auxi++;
       }
    _aux = false;
    while (!_aux) 
    {
        if (abs(pos.getY()- posMat[auxi][auxj].getY()) < auxtiledsize.getY()) {
         if (abs(posMat[auxi][auxj].getY() - pos.getY()) > abs(posMat[auxi][auxj+1].getY() - pos.getY())) {
            auxj++;
            _aux = true;
            }
        else {
            _aux = true;
        }
        }
        else  auxj++;
    }
    return posMat[auxi][auxj];
}

void DragPuzzleScene::Exit()
{
    Game::Instance()->getSceneManager()->popScene();
}

bool DragPuzzleScene::Check()
{
    Win();
    return true;
}

void DragPuzzleScene::Win()
{
    entityManager->removeComponent<TriggerComponent>(_triggerObj);
    solved = true;
    // WIP active de respective part in room2 scene
    std::cout << "WEIN";
}
