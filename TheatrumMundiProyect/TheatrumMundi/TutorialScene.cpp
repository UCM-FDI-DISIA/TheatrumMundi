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
#include "InvAnimComponent.h"
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
			else if(dialogCount == 4 || dialogCount == 5){}//Do nothing
			else {
				HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
				Game::Instance()->getSceneManager()->loadScene(TELE_PUZZLE, this);
			}
		};
	roomEvent[Antenna] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("antena", sdlutils().invDescriptions().at("antena"), &sdlutils().images().at("antena")));
		GetInventory()->hints.push_back(entityFactory->CreateInvEntity(entityManager, "antena", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		createDescription(GetInventory()->hints.back(), GetInventory()->getItems().back());
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
		//AudioManager& a = AudioManager::Instance();

		audioManager().stopSound(sdlutils().musics().at("menu"));

		std::shared_ptr<Sound> music = sdlutils().musics().at("intermedia");
		audioManager().playSound(music, true);

		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		audioManager().setVolume(buttonSound, 0.2);
		std::shared_ptr<Sound> puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		audioManager().setVolume(puzzleButtonSound, 0.3);

		std::shared_ptr<Sound> doorSound = sdlutils().soundEffects().at("puerta");
		audioManager().setVolume(doorSound, 0.6);
		audioManager().set3DPosition(doorSound, -500, 0, 0);

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
				if(startRoomScroll->Scroll(ScrollComponent::RIGHT)) audioManager().playSound(doorSound);
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
			audioManager().playSound(puzzleButtonSound);
			roomEvent[Dialog2]();
			entityManager->setActive(passwordButton, false);
			entityManager->setActive(doorImage, true);
			});

		entityManager->setActive(passwordButton, false);


		//Antenna
		antenna = entityFactory->CreateInteractableEntity(entityManager, "antena", EntityFactory::RECTAREA, Vector2D(900, 630), Vector2D(0, 0), 133, 85, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		//startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(antenna));
		entityManager->getComponent<ClickComponent>(antenna)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			audioManager().playSound(puzzleButtonSound);
			entityManager->removeEntity(antenna);
			entityManager->setActive(antenna, false);
			roomEvent[Antenna]();
			});
		entityManager->setActive(antenna, false);
		

		//Television
		television = entityFactory->CreateInteractableEntity(entityManager, "teleSinAntena", EntityFactory::RECTAREA, Vector2D(480 - 1349, 200), Vector2D(0, 0), 385, 498, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		startRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(television));
		entityManager->getComponent<ClickComponent>(television)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			audioManager().playSound(puzzleButtonSound);
			roomEvent[TeleScene]();
			});

		television->getMngr()->getComponent<ClickComponent>(television)->setActive(false);

		//INVENTORY

		invObjects.InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
		entityManager->addComponent<InvAnimComponent>(invObjects.InventoryBackground);
		inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(invObjects.InventoryBackground, false);

		invObjects.InvArea = entityManager->addComponent<RectArea2D>(invObjects.InventoryBackground, areaLayerManager);

		invObjects.inventoryUpButton = entityFactory->CreateInvEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(invObjects.inventoryUpButton, false);

		invObjects.inventoryDownButton = entityFactory->CreateInvEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(invObjects.inventoryDownButton, false);
		entityManager->getComponent<ClickComponent>(inventoryButton)
			->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
				{
					audioManager().playSound(buttonSound);
					GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

					if (GetInventory()->getActive()) // If the inventory is active, activate the items
					{
						entityManager->setActive(invObjects.InventoryBackground, true);
						entityManager->getComponent<InvAnimComponent>(invObjects.InventoryBackground)->startInvAnim();
						entityManager->setActive(logbtn, false);
						//change the position of the log button
						areaLayerManager->sendFront(invObjects.InvArea->getLayerPos());
						

						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryUpButton)->getLayerPos());
						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryDownButton)->getLayerPos());

						entityManager->setActive(invObjects.inventoryDownButton, true);
						entityManager->setActive(invObjects.inventoryUpButton, true);

						entityManager->getComponent<InvAnimComponent>(invObjects.inventoryDownButton)->startInvAnim();
						entityManager->getComponent<InvAnimComponent>(invObjects.inventoryUpButton)->startInvAnim();

						for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber() + GetInventory()->getFirstItem(); ++i) {
							GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
							areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inv->hints[i])->getLayerPos());
							inv->hints[i]->getMngr()->getComponent<InvAnimComponent>(inv->hints[i])->startInvAnim();
						}
					
						if (dialogCount == 5) {
							roomEvent[Dialog6]();
						}

					}
					else
					{
						entityManager->setActive(logbtn, true);
						entityManager->getComponent<InvAnimComponent>(invObjects.inventoryDownButton)->endInvAnim();
						entityManager->getComponent<InvAnimComponent>(invObjects.inventoryUpButton)->endInvAnim();
						entityManager->getComponent<InvAnimComponent>(invObjects.InventoryBackground)->endInvAnim();
						
						for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i) GetInventory()->hints[i]->getMngr()->getComponent<InvAnimComponent>(inv->hints[i])->endInvAnim();

						

					}
				});

		entityManager->setActive(inventoryButton, false);

		entityManager->getComponent<ClickComponent>(invObjects.inventoryUpButton)
			->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
				{
					audioManager().playSound(buttonSound);
					scrollInventory(-1);
				});
		entityManager->getComponent<ClickComponent>(invObjects.inventoryDownButton)
			->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]()
				{
					audioManager().playSound(buttonSound);
					scrollInventory(1);
				});
		invObjects.textDescriptionEnt = entityManager->addEntity(ecs::grp::UI);
		auto _testTextTranform = entityManager->addComponent<Transform>(invObjects.textDescriptionEnt, Vector2D(600, 300), Vector2D(0, 0), 300, 200, 0);
		entityManager->setActive(invObjects.textDescriptionEnt, false);
		SDL_Color colorDialog = { 255, 255, 255, 255 };
		entityManager->addComponent<WriteTextComponent<DescriptionInfo>>(invObjects.textDescriptionEnt, sdlutils().fonts().at("BASE"), colorDialog, GetInventory()->getTextDescription());
		
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
