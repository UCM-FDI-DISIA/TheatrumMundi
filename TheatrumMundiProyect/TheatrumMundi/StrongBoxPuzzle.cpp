#include "StrongBoxPuzzle.h"
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
StrongBoxPuzzle::StrongBoxPuzzle()
{
   dialogueManager = new DialogueManager(1);
   rotSol.push_back(0);
   rotSol.push_back(0);
   rotSol.push_back(0);
   rotSol.push_back(0);
}
StrongBoxPuzzle::~StrongBoxPuzzle()
{
}
void StrongBoxPuzzle::init()
//void StrongBoxPuzzle::init(SceneRoomTemplate* sr)
{
    SceneRoomTemplate* sr = nullptr;
    if (!isStarted) {
        room = nullptr;
        
        isStarted = true;
        //sound and music
        AudioManager& a = AudioManager::Instance();
        Sound buttonSound = sdlutils().soundEffects().at("boton");
        a.setVolume(buttonSound, 0.2);
       



        //strongBox 
        auto big = entityFactory->CreateInteractableEntity(entityManager, "CosaQueArrastras", EntityFactory::CIRCLEAREA, Vector2D(387, 86.5), Vector2D(0, 0), 575, 575, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux3 = entityManager->addComponent<SlowRotateComponent>(big, entityManager->getComponent<Transform>(big));
        entityManager->getComponent<ClickComponent>(big)->connect(ClickComponent::JUST_CLICKED, [this, aux3]() {
            aux3->startRotate(60);
            });

        auto medium = entityFactory->CreateInteractableEntity(entityManager, "CosaQueArrastras", EntityFactory::CIRCLEAREA, Vector2D(449.5, 149), Vector2D(0, 0), 450, 450, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux2 = entityManager->addComponent<SlowRotateComponent>(medium, entityManager->getComponent<Transform>(medium));
        entityManager->getComponent<ClickComponent>(medium)->connect(ClickComponent::JUST_CLICKED, [this, aux2]() {
            aux2->startRotate(72);
            });
        auto small = entityFactory->CreateInteractableEntity(entityManager, "CosaQueArrastras", EntityFactory::CIRCLEAREA, Vector2D(512, 211.5), Vector2D(0, 0), 325, 325, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux = entityManager->addComponent<SlowRotateComponent>(small,entityManager->getComponent<Transform>(small));
        entityManager->getComponent<ClickComponent>(small)->connect(ClickComponent::JUST_CLICKED, [this,aux](){
            aux->startRotate(90);
            });
        auto smallest = entityFactory->CreateInteractableEntity(entityManager, "CosaQueArrastras", EntityFactory::CIRCLEAREA, Vector2D(574.5, 274), Vector2D(0, 0), 200, 200, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux1 = entityManager->addComponent<SlowRotateComponent>(smallest, entityManager->getComponent<Transform>(smallest));
        entityManager->getComponent<ClickComponent>(smallest)->connect(ClickComponent::JUST_CLICKED, [this, aux1]() {
            aux1->startRotate(120);
            });
/*
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
                HideInventoryItems(InventoryBackground, downButton, upButton, sr);
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
        */
    }
   // createInvEntities(sr);
}

void StrongBoxPuzzle::unload()
{
}

bool StrongBoxPuzzle::Check()
{
    return false;
}

void StrongBoxPuzzle::Win()
{
}

void StrongBoxPuzzle::ResolveScene()
{
}
