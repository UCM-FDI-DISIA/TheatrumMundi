#include "TutorialScene.h"
#include <list>
#include "../src/utils/Vector2D.h"
#include "../src/components/Transform.h"
#include "../src/components/Image.h"
#include "../../TheatrumMundiProyect/src/sdlutils/SDLUtils.h"
#include "LogComponent.h"
#include "../TheatrumMundi/PhysicsBodyComponent.h"
#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "../src/Components/ScrollComponent.h"
#include "../src/ecs/Manager.h"
#include "../src/game/Game.h"
#include "ClickableSpriteComponent.h"
#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"



#include "../src/components/WriteTextComponent.h"
#include "DialogueManager.h"


#include "GameSave.h"

TutorialScene::TutorialScene() : SceneRoomTemplate()
{
	ChangeRoom1 = nullptr;
	doorImage = nullptr;
	passwordButton = nullptr;
	botonBack = nullptr;
	television = nullptr;
	antenna = nullptr;
	dialogCount = -1;

	dialogueManager = new DialogueManager(-1);
	roomEvent.resize(event_size);
	roomEvent[Dialog0] = [this]
		{
			startDialogue("Tutorial0");

		};
	roomEvent[Dialog1] = [this]
		{
			startDialogue("Tutorial1");

		};
	roomEvent[Dialog2] = [this]
		{
			startDialogue("Tutorial2");

		};
	roomEvent[Dialog3] = [this]
		{
			startDialogue("Tutorial3");

		};

	roomEvent[Dialog5] = [this]
		{
			startDialogue("Tutorial5");

		};
	roomEvent[Dialog6] = [this]
		{
			startDialogue("Tutorial6");

		};
	roomEvent[TeleScene] = [this]
		{
			if (dialogCount == 3) {
				dialogCount += 1;
				entityManager->setActive(antenna, true);
				HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
				Game::Instance()->getSceneManager()->loadScene(TELE_PUZZLE, this);
			}
			else if (dialogCount == 4) {
				std::cout << "dialogo 4 no puedo entrar" << std::endl;
				return;
			}
			else if (dialogCount == 5) {
				std::cout << "dialogo 5 no puedo entrar" << std::endl;
				return;
			}
			else 
				Game::Instance()->getSceneManager()->loadScene(TELE_PUZZLE, this);
			
		};
	roomEvent[Antenna] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("antena", "Antena de un televisor. ¿Esto sigue existiendo?", &sdlutils().images().at("antena")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "antena", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		roomEvent[Dialog5]();
		};

}

TutorialScene::~TutorialScene()
{
}

void TutorialScene::init()
{

	if (!isStarted) {
		isStarted = true;
		finishallpuzzles = false;
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);
		std::shared_ptr<Sound> puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		a.setVolume(puzzleButtonSound, 0.3);

		std::shared_ptr<Sound> doorSound = sdlutils().soundEffects().at("puerta");
		a.setVolume(doorSound, 0.6);
		a.set3DPosition(doorSound, -500, 0, 0);

		//Register scene in dialogue manager
		dialogueManager->setScene(this);
		dialogCount = -1;

		//Background and Scene scroll
		ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	

		auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);

		ChangeRoomScroll->setEndScrollCallback([this](){ 
			entityManager->setActive(doorImage, false);
			roomEvent[Dialog3](); 
			
			});

		auto startRoom = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		
		auto startRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(startRoom));


		auto secondRoom = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(-1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(secondRoom));

		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, startRoomScroll, ChangeRoomScroll, doorSound]() {
			if (!startRoomScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				startRoomScroll->Scroll(ScrollComponent::RIGHT);
				
			
				
			}
			});

		entityManager->setActive(ChangeRoom1, false);

		//door image
		doorImage = entityFactory->CreateImageEntity(entityManager, "TutorialPuerta", Vector2D(34, 215), Vector2D(0, 0), 136, 495, 0, ecs::grp::DEFAULT);
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(doorImage));
		entityManager->setActive(doorImage, false);

		//password button
		passwordButton = entityFactory->CreateInteractableEntity(entityManager, "botonNumero", EntityFactory::RECTAREA, Vector2D(550, 300), Vector2D(0, 0), 200, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		entityManager->getComponent<ClickComponent>(passwordButton)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[Dialog2]();
			entityManager->setActive(passwordButton, false);
			entityManager->setActive(doorImage, true);
			});

		entityManager->setActive(passwordButton, false);


		//Antenna
		antenna = entityFactory->CreateInteractableEntity(entityManager, "antena", EntityFactory::RECTAREA, Vector2D(900, 630), Vector2D(0, 0), 133, 85, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		//startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(antenna));
		entityManager->getComponent<ClickComponent>(antenna)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			entityManager->removeEntity(antenna);
			entityManager->setActive(antenna, false);
			roomEvent[Antenna]();
			});
		entityManager->setActive(antenna, false);
		

		//Television
		television = entityFactory->CreateInteractableEntity(entityManager, "teleSinAntena", EntityFactory::RECTAREA, Vector2D(480 - 1349, 200), Vector2D(0, 0), 385, 498, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(television));
		entityManager->getComponent<ClickComponent>(television)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[TeleScene]();
			});

		television->getMngr()->getComponent<ClickComponent>(television)->setActive(false);

		//entityManager->setActive(television,false);

		


		//quit button
		/*
		botonBack = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 270 / 4, 270 / 4, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		entityManager->getComponent<ClickComponent>(botonBack)->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {

			AudioManager::Instance().playSound(buttonSound);
			Game::Instance()->getSceneManager()->loadScene(INITIAL_MENU);
			});

		entityManager->setActive(botonBack, false);
		*/

		//INVENTORY

		invObjects.InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);

		inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(invObjects.InventoryBackground, false);

		invObjects.InvArea = entityManager->addComponent<RectArea2D>(invObjects.InventoryBackground, areaLayerManager);

		invObjects.inventoryUpButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(invObjects.inventoryUpButton, false);

		invObjects.inventoryDownButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(invObjects.inventoryDownButton, false);

		entityManager->getComponent<ClickComponent>(inventoryButton)
			->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
				{
					AudioManager::Instance().playSound(buttonSound);
					GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

					if (GetInventory()->getActive()) // If the inventory is active, activate the items
					{
						entityManager->setActive(logbtn, false);
						entityManager->setActive(invObjects.InventoryBackground, true);
						//change the position of the log button
						areaLayerManager->sendFront(invObjects.InvArea->getLayerPos());
						entityManager->getComponent<Transform>(inventoryButton)->setPosX(925);

						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryUpButton)->getLayerPos());
						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryDownButton)->getLayerPos());

						entityManager->setActive(invObjects.inventoryDownButton, true);
						entityManager->setActive(invObjects.inventoryUpButton, true);

						for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber() + GetInventory()->getFirstItem(); ++i) GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
					
						if (dialogCount == 5) {
							roomEvent[Dialog6]();
						}

					}
					else
					{
						entityManager->setActive(logbtn, true);
						entityManager->setActive(invObjects.InventoryBackground, false);
						entityManager->setActive(invObjects.inventoryDownButton, false);
						entityManager->setActive(invObjects.inventoryUpButton, false);
						inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);
						for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i) GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Activate the items

						

					}
				});

		entityManager->setActive(inventoryButton, false);

		entityManager->getComponent<ClickComponent>(invObjects.inventoryUpButton)
			->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
				{
					AudioManager::Instance().playSound(buttonSound);
					scrollInventory(-1);
				});
		entityManager->getComponent<ClickComponent>(invObjects.inventoryDownButton)
			->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
				{
					AudioManager::Instance().playSound(buttonSound);
					scrollInventory(1);
				});

		
		//INIT DIALOG MANAGER
		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, _eventToRead);

		
		roomEvent[Dialog0]();

	}
	

}

void TutorialScene::resolvedPuzzle(int i)
{
}

void TutorialScene::refresh()
{
}


void TutorialScene::unload()
{
	entityManager->~EntityManager();
}


void TutorialScene::endDialogue()
{
	dialogCount += 1;
	#ifdef DEBUG
		std::cout << "acabo dialogo numero: " << dialogCount << std::endl;
	#endif // DEBUG

	switch (dialogCount) {
		case 0:
			dialogueManager->setdisplayOnProcess(false);
		
			
			logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);

			Game::Instance()->getLog()->addDialogueLineLog(" ", "PW: 3711");

			//entityManager->setActive(botonBack, true);
			

			break;
		case 1:
			dialogueManager->setdisplayOnProcess(false);

			

			entityManager->setActive(passwordButton, true);
			
			break;

		case 2:
			dialogueManager->setdisplayOnProcess(false);

			
			entityManager->setActive(ChangeRoom1, true);
			
			break;

		case 3:
			dialogueManager->setdisplayOnProcess(false);
			
			television->getMngr()->getComponent<ClickComponent>(television)->setActive(true);
			
			break;

		case 5:
			dialogueManager->setdisplayOnProcess(false);
			
			entityManager->setActive(inventoryButton, true);
			break;

		case 6:
			dialogueManager->setdisplayOnProcess(false);
		

			break;


	}
	
}

void TutorialScene::closedLog()
{
	if (dialogCount == 0) {
		roomEvent[Dialog1]();
	}
}
