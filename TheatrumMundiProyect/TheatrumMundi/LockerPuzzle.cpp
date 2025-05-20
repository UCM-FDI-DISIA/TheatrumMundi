#include "LockerPuzzle.h"
#include "Vector2D.h"
#include "SDLUtils.h"
#include "DialogueManager.h"
#include "DragComponent.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "TiledAreaComponent.h"
#include "SceneRoomTemplate.h"
#include "../ecs/Manager.h"
#include "Log.h"
#include <list>
#include "Area2D.h"
#include "SlowRotateComponent.h"
#include "RectArea2D.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "Image.h"

//void StrongBoxPuzzle::init()
LockerPuzzle::LockerPuzzle()
{
   dialogueManager = new DialogueManager(1);
   rotSol.push_back(0);
   rotSol.push_back(0);
   rotSol.push_back(0);
   rotSol.push_back(0);
}
LockerPuzzle::~LockerPuzzle()
{
}
void LockerPuzzle::init(SceneRoomTemplate* sr)
//void StrongBoxPuzzle::init(SceneRoomTemplate* sr)
{
    
    if (!isStarted) {
        room = sr;
        
        isStarted = true;
        //sound and music
        AudioManager& a = AudioManager::Instance();
        std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
        a.setVolume(buttonSound, 0.2);
       
        entityFactory->CreateImageEntity(entityManager, "lockFondoCerrado", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);


        //strongBox door
        //Big wheel 
        auto big = entityFactory->CreateInteractableEntity(entityManager, "StrongBoxWheel", EntityFactory::CIRCLEAREA, Vector2D(387, 86.5), Vector2D(0, 0), 575, 575, 60, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux3 = entityManager->addComponent<SlowRotateComponent>(big, entityManager->getComponent<Transform>(big));
        entityManager->getComponent<ClickComponent>(big)->connect(ClickComponent::JUST_CLICKED, [this, aux3]() {
            aux3->startRotate(60);
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(big));
        doorEntities.push_back(big);
        
        //medium wheel 
        auto medium = entityFactory->CreateInteractableEntity(entityManager, "StrongBoxWheel", EntityFactory::CIRCLEAREA, Vector2D(449.5, 149), Vector2D(0, 0), 450, 450, 288, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux2 = entityManager->addComponent<SlowRotateComponent>(medium, entityManager->getComponent<Transform>(medium));
        entityManager->getComponent<ClickComponent>(medium)->connect(ClickComponent::JUST_CLICKED, [this, aux2]() {
            aux2->startRotate(72);
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(medium));
        doorEntities.push_back(medium);
        
        //small wheel
        auto small = entityFactory->CreateInteractableEntity(entityManager, "StrongBoxWheel", EntityFactory::CIRCLEAREA, Vector2D(512, 211.5), Vector2D(0, 0), 325, 325, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux = entityManager->addComponent<SlowRotateComponent>(small,entityManager->getComponent<Transform>(small));
        entityManager->getComponent<ClickComponent>(small)->connect(ClickComponent::JUST_CLICKED, [this,aux](){
            aux->startRotate(90);
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(small));
        doorEntities.push_back(small);
       
        //smallest wheel
        auto smallest = entityFactory->CreateInteractableEntity(entityManager, "StrongBoxWheel", EntityFactory::CIRCLEAREA, Vector2D(574.5, 274), Vector2D(0, 0), 200, 200, 240, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux1 = entityManager->addComponent<SlowRotateComponent>(smallest, entityManager->getComponent<Transform>(smallest));
        entityManager->getComponent<ClickComponent>(smallest)->connect(ClickComponent::JUST_CLICKED, [this, aux1]() {
            aux1->startRotate(120);
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(smallest));
        doorEntities.push_back(smallest);
       

        auto checkbtn = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(10, 200), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        entityManager->getComponent<ClickComponent>(checkbtn)->connect(ClickComponent::JUST_CLICKED, [this]() {
            if (Check()) {
                Win();
            }
            });
        doorEntities.push_back(checkbtn);

       //INVENTORY
       createInventoryUI();

       //BackButton
       auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);


       //Click component Open log button
       ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
       clkOpen->connect(ClickComponent::JUST_CLICKED, [this, _backButton, buttonSound]()
           {
               AudioManager::Instance().playSound(buttonSound);
               inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
               HideInventoryItems();
               room->GetInventory()->setFirstItem(0);
               auto _backButtonImage = _backButton->getMngr()->getComponent<Image>(_backButton);
               _backButtonImage->setW(_backButton->getMngr()->getComponent<Transform>(_backButton)->getWidth());
               _backButtonImage->setH(_backButton->getMngr()->getComponent<Transform>(_backButton)->getHeight());
               _backButtonImage->setPosOffset(0, 0);
               Game::Instance()->getSceneManager()->popScene();
           }); 

        dialogueManager->Init(1, entityFactory, entityManager, false, areaLayerManager, "SalaIntermedia1");
        logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);

        //IMPORTANT this need to be out of the isstarted!!!
        sr->GetInventory()->setFirstItem(0);
        createInvEntities(sr);
        dialogueManager->setScene(this);
        startDialogue("PuzzleLibros");
        
        //Puzzle rewards
        //Wire
        int variant = Game::Instance()->getDataManager()->GetRoomVariant(2);
        ecs::entity_t rewardWire;
        if(variant==2 )rewardWire = entityFactory->CreateInteractableEntity(entityManager, "alambre", EntityFactory::RECTAREA,
            Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
            areaLayerManager,
            EntityFactory::NODRAG,
            ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
        else rewardWire = entityFactory->CreateInteractableEntity(entityManager, "soga", EntityFactory::RECTAREA,
            Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
            areaLayerManager,
            EntityFactory::NODRAG,
            ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
        ClickComponent* clk = entityManager->getComponent<ClickComponent>(rewardWire);
        clk->connect(ClickComponent::JUST_CLICKED, [this, rewardWire, variant, sr]() {

            Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
            if (variant == 0)AddInvItem("soga", sdlutils().Instance()->invDescriptions().at("Soga"), position, sr);
            else if (variant == 1)AddInvItem("soga", sdlutils().Instance()->invDescriptions().at("Soga"), position, sr);
            else if (variant == 2)AddInvItem("alambre", sdlutils().Instance()->invDescriptions().at("Alambre"), position, sr);
            rewardWire->getMngr()->setActive(rewardWire, false);

            });
        rewardEntities.push_back(rewardWire);
        entityManager->setActive(rewardWire, false);
       

    }
   // createInvEntities(sr);
}

void LockerPuzzle::unload()
{
}

bool LockerPuzzle::Check()
{
    bool flag = true;
    for (int i = 0; i < rotSol.size();i++) if (rotSol[i] != wheelstr[i]->getRot()) flag = false;
    return flag;
}

void LockerPuzzle::Win()
{
    for (auto a : doorEntities)entityManager->setActive(a, false);
    for (auto a : rewardEntities)entityManager->setActive(a, true);

    room->resolvedPuzzle(2);
}

void LockerPuzzle::ResolveScene()
{
    Win();
}
