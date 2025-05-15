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
       //Invntory Background
        auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::DEFAULT);
        entityManager->setActive(InventoryBackground, false);

        auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
        entityManager->setActive(upButton, false);

        auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
        entityManager->setActive(downButton, false);

        //InventoryButton
        auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
        ClickComponent* invOpen = entityManager->addComponent<ClickComponent>(inventoryButton);
        invOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, upButton, downButton, inventoryButton, buttonSound]() //Lamda function
            {
                AudioManager::Instance().playSound(buttonSound);
                sr->GetInventory()->setActive(!sr->GetInventory()->getActive());  // Toggle the inventory

                // If the inventory is active, activate the items
                if (sr->GetInventory()->getActive()) {
                    entityManager->setActive(InventoryBackground, true);
                    entityManager->setActive(logbtn, false);

                    inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(925);
                    entityManager->setActive(downButton, true);
                    entityManager->setActive(upButton, true);

                    for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
                        invObjects[i]->getMngr()->setActive(invObjects[i], true);
                    }

                }
                else {
                    entityManager->setActive(InventoryBackground, false);
                    entityManager->setActive(logbtn, true);
                    entityManager->setActive(downButton, false);
                    entityManager->setActive(upButton, false);
                    inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

                    for (int i = sr->GetInventory()->getFirstItem(); i < sr->GetInventory()->getFirstItem() + sr->GetInventory()->getItemNumber(); ++i) {
                        invObjects[i]->getMngr()->setActive(invObjects[i], false);
                    }
                }
            });

        ClickComponent* UPbuttonInventoryClick = entityManager->getComponent<ClickComponent>(upButton);
        UPbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, upButton, sr]() {

            AudioManager::Instance().playSound(buttonSound);
            scrollInventoryPuzzle(-1, sr);
            });

        ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
        DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton, sr]() {

            AudioManager::Instance().playSound(buttonSound);
            scrollInventoryPuzzle(1, sr);
            });

        //BackButton
        auto _backButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::BOOKS_PUZZLE_SCENE_INTERACTABLE_INITIAL);

        //Click component Open log button
        ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_backButton);
        clkOpen->connect(ClickComponent::JUST_CLICKED, [this, sr, InventoryBackground, downButton, upButton, inventoryButton, _backButton, buttonSound]()
            {
                AudioManager::Instance().playSound(buttonSound);
                inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
                HideInventoryItems();
                sr->GetInventory()->setFirstItem(0);
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
        //feather
        ecs::entity_t rewardFeather = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA,
            Vector2D(560, 630), Vector2D(0, 0), 110, 110, 0,
            areaLayerManager,
            EntityFactory::NODRAG,
            ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
        int variant = Game::Instance()->getDataManager()->GetRoomVariant(2);
        ClickComponent* clk = entityManager->getComponent<ClickComponent>(rewardFeather);
        clk->connect(ClickComponent::JUST_CLICKED, [this, rewardFeather, variant, sr]() {

            Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
            if (variant == 0)AddInvItem("frascoV2", sdlutils().Instance()->invDescriptions().at("frascoV2"), position, sr);
            else if (variant == 1)AddInvItem("frascoV2", sdlutils().Instance()->invDescriptions().at("frascoV1-1"), position, sr);
            else if (variant == 2)AddInvItem("frascoV1", sdlutils().Instance()->invDescriptions().at("frascoV1"), position, sr);
            rewardFeather->getMngr()->setActive(rewardFeather, false);

            });
        rewardEntities.push_back(rewardFeather);
        entityManager->setActive(rewardFeather, false);
        //morse book
        ecs::entity_t rewardMorse = entityFactory->CreateInteractableEntity(entityManager, "B5", EntityFactory::RECTAREA,
            Vector2D(660, 630), Vector2D(0, 0), 110, 110, 0,
            areaLayerManager,
            EntityFactory::NODRAG,
            ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
        clk = entityManager->getComponent<ClickComponent>(rewardMorse);
        clk->connect(ClickComponent::JUST_CLICKED, [this, rewardMorse, variant, sr]() {

            Vector2D position = sr->GetInventory()->setPosition(); //Position of the new object
            if (variant == 0)AddInvItem("frascoV2", sdlutils().Instance()->invDescriptions().at("frascoV2"), position, sr);
            else if (variant == 1)AddInvItem("frascoV2", sdlutils().Instance()->invDescriptions().at("frascoV1-1"), position, sr);
            else if (variant == 2)AddInvItem("frascoV1", sdlutils().Instance()->invDescriptions().at("frascoV1"), position, sr);
            rewardMorse->getMngr()->setActive(rewardMorse, false);

            });
        rewardEntities.push_back(rewardMorse);
        entityManager->setActive(rewardMorse, false);

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
