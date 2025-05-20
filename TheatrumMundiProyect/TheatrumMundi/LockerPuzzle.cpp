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
   rotSol.push_back(240);
   rotSol.push_back(72);
   rotSol.push_back(0);
   rotSol.push_back(180);
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
       
        background = entityFactory->CreateImageEntity(entityManager, "lockFondoCerrado", Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0, ecs::grp::DEFAULT);


        //strongBox door
        //Big wheel 
        auto big = entityFactory->CreateInteractableEntity(entityManager, "rueda1", EntityFactory::CIRCLEAREA, Vector2D(755, 370), Vector2D(0, 0), 516/1.5, 516 / 1.5, 60, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux3 = entityManager->addComponent<SlowRotateComponent>(big, entityManager->getComponent<Transform>(big));
        aux3->setEndRotEvent([this]() {
            if (Check()) {
                Win();
            }
            });
        entityManager->getComponent<ClickComponent>(big)->connect(ClickComponent::JUST_CLICKED, [this, aux3]() {
            aux3->startRotate(60);
           
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(big));
        doorEntities.push_back(big);
        
        //medium wheel 
        auto medium = entityFactory->CreateInteractableEntity(entityManager, "rueda2", EntityFactory::CIRCLEAREA, Vector2D(795, 414), Vector2D(0, 0), 395/1.5, 395 / 1.5, 288, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux2 = entityManager->addComponent<SlowRotateComponent>(medium, entityManager->getComponent<Transform>(medium));
        aux2->setEndRotEvent([this]() {
            if (Check()) {
                Win();
            }
            });
        entityManager->getComponent<ClickComponent>(medium)->connect(ClickComponent::JUST_CLICKED, [this, aux2]() {
            aux2->startRotate(72);
            
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(medium));
        doorEntities.push_back(medium);
        
        //small wheel
        auto small = entityFactory->CreateInteractableEntity(entityManager, "rueda3", EntityFactory::CIRCLEAREA, Vector2D(837, 453), Vector2D(0, 0), 274/1.5, 274 / 1.5, 120, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux = entityManager->addComponent<SlowRotateComponent>(small,entityManager->getComponent<Transform>(small));
        aux->setEndRotEvent([this]() {
            if (Check()) {
                Win();
            }
            });
        entityManager->getComponent<ClickComponent>(small)->connect(ClickComponent::JUST_CLICKED, [this,aux](){
            aux->startRotate(120);
           
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(small));
        doorEntities.push_back(small);
       
        //smallest wheel
        auto smallest = entityFactory->CreateInteractableEntity(entityManager, "rueda4", EntityFactory::CIRCLEAREA, Vector2D(876, 490), Vector2D(0, 0), 158/1.5, 158 / 1.5, 180, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
        SlowRotateComponent* aux1 = entityManager->addComponent<SlowRotateComponent>(smallest, entityManager->getComponent<Transform>(smallest));
        aux1->setEndRotEvent([this]() {
            if (Check()) {
                Win();
            }
            });
        entityManager->getComponent<ClickComponent>(smallest)->connect(ClickComponent::JUST_CLICKED, [this, aux1]() {
            aux1->startRotate(180);
            
            });
        wheelstr.push_back(entityManager->getComponent<Transform>(smallest));
        doorEntities.push_back(smallest);
       


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
            Vector2D(65, 30), Vector2D(0, 0), 310, 310, 0,
            areaLayerManager,
            EntityFactory::NODRAG,
            ecs::grp::BOOKS_PUZZLE_SCENE_REWARD);
        else rewardWire = entityFactory->CreateInteractableEntity(entityManager, "soga", EntityFactory::RECTAREA,
            Vector2D(650, 30), Vector2D(0, 0), 310, 310, 0,
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
    for (int i = 0; i < rotSol.size();i++) if (rotSol[i] != wheelstr[i]->getRot()) { 
        std::cout << i << "ruedafallo" << std::endl;
        flag = false; }

    return flag;
}

void LockerPuzzle::Win()
{
    background->getMngr()->getComponent<Image>(background)->setTexture(&sdlutils().images().at("lockFondoAbierto"));

    for (auto a : rewardEntities)entityManager->setActive(a, true);

    room->resolvedPuzzle(2);
}

void LockerPuzzle::ResolveScene()
{
    Win();
}
