#include "roomprueba.h"
#include "DialogueManager.h"
#include "ScrollComponent.h"
#include "ClickComponent.h"
#include "Game.h"
#include "Log.h"
#include "RectArea2D.h"
roomprueba::roomprueba()
{
	//Creation of the DialogueManager of the room and creation of the events 
	dialogueManager = new DialogueManager(1);
	roomEvent.resize(event_size);
#pragma region Events
	roomEvent[InitialDialogue] = [this] {
		startDialogue("SalaIntermedia2");
		};
	roomEvent[BalancePuzzleScene] = [this] {
		//Game::Instance()->getSceneManager()->loadScene(BALANCE_PUZZLE, this);
		};
	roomEvent[WiresPuzzleScene] = [this] {
		//Game::Instance()->getSceneManager()->loadScene(WIRES_PUZZLE, this);
		};
	roomEvent[BalanceSceneRsv] = [this] {

#pragma endregion
		};
}

roomprueba::~roomprueba()
{
}

void roomprueba::init()
{
	if (!isStarted) {

		isStarted = true;
		isOpen = false;
		finishallpuzzles = false;
		dialogueManager->setScene(this);
		Game::Instance()->getDataManager()->SetSceneCount(ROOM3);
		int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(2);

#pragma region Audio

		//Audio sfx 
	//	AudioManager& a = AudioManager::Instance();
		std::shared_ptr<Sound> buttonSound = sdlutils().soundEffects().at("boton");
		audioManager().setVolume(buttonSound, 0.2);
		std::shared_ptr<Sound> puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		audioManager().setVolume(puzzleButtonSound, 0.3);
		std::shared_ptr<Sound> doorSound = sdlutils().soundEffects().at("puerta");
		//Audio music
		//Sound room2music = sdlutils().musics().at("room2music");
		//a.setLooping(room2music, true);
		//a.playSound(room2music);

#pragma endregion

#pragma region InitScroll

		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1200, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(22, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto CementeryBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom2);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom1));
		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);

#pragma endregion

#pragma region UI

		//Quit
		_quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(_quitButton)->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			audioManager().playSound(buttonSound);
			entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
			entityManager->setActive(_quitButton, false);
			entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			if (!puzzlesol[2]) {
				//mirror->getMngr()->setActive(mirror, false);
				//secretEntry->getMngr()->setActive(secretEntry, false);
			}
			});
		entityManager->setActive(_quitButton, false);

		//Pause
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			audioManager().playSound(buttonSound);
			});

#pragma region Inventory

		//Inventory
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);

		auto inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(InventoryBackground, false);

		auto InvArea = entityManager->addComponent<RectArea2D>(InventoryBackground, areaLayerManager);

		auto inventoryUpButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(inventoryUpButton, false);

		auto inventoryDownButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(inventoryDownButton, false);

		entityManager->getComponent<ClickComponent>(inventoryButton)
			->connect(ClickComponent::JUST_CLICKED, [this, InventoryBackground, inventoryUpButton, inventoryDownButton, InvArea, inventoryButton, buttonSound]()
				{
					audioManager().playSound(buttonSound);
					GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

					if (GetInventory()->getActive()) // If the inventory is active, activate the items
					{
						entityManager->setActive(InventoryBackground, true);

						entityManager->getComponent<Transform>(inventoryButton)->setPosX(925);

						//change the position of the log button
						areaLayerManager->sendFront(InvArea->getLayerPos());

						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inventoryUpButton)->getLayerPos());
						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inventoryDownButton)->getLayerPos());

						entityManager->setActive(inventoryDownButton, true);
						entityManager->setActive(inventoryUpButton, true);

						for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i)
							GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
					}
					else
					{
						entityManager->setActive(InventoryBackground, false);
						entityManager->setActive(inventoryDownButton, false);
						entityManager->setActive(inventoryUpButton, false);

						inventoryButton->getMngr()->getComponent<Transform>(inventoryButton)->setPosX(60 + 268 / 3);

						// its okay to use the first item as the first item to show??
						for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i)
							GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Desactivate the hints
					}
				});
#pragma endregion

		//Log

		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, _eventToRead);

		logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);

		//startDialogue("Room2");


#pragma endregion

#pragma region Background

		//LeftBackground
		auto WaitRoomBackground = entityFactory->CreateImageEntity(entityManager, "salaEspera", Vector2D(-10, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::DEFAULT);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(WaitRoomBackground));

		//RightBackground
		auto MausoleumBackground = entityFactory->CreateImageEntity(entityManager, "juzgado", Vector2D(1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(MausoleumBackground));


#pragma endregion


#pragma endregion

#pragma region JudgeEntities

		auto balance = entityFactory->CreateInteractableEntity(entityManager, "balance", EntityFactory::RECTAREA, Vector2D(400, 300), Vector2D(0, 0), 250 / 3, 225 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(balance));
		entityManager->getComponent<ClickComponent>(balance)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[BalancePuzzleScene]();
			});

		auto boa = entityFactory->CreateInteractableEntity(entityManager, "boa1", EntityFactory::RECTAREA, Vector2D(700, 300), Vector2D(0, 0), 250 / 3, 225 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(boa));
		entityManager->getComponent<ClickComponent>(boa)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[WiresPuzzleScene]();
			});

	}
	SDL_Delay(1000);

}

void roomprueba::resolvedPuzzle(int i)
{
	if (i < 5) {
		int auxevent = event_size;
		if (i == 0)  auxevent = BalanceSceneRsv;
		//else if (i == 1)  auxevent = RavenSceneRsv;
		//else if (i == 2)  auxevent = DoorSceneRsv;
		//else if (i == 3)  auxevent = MosaicPuzzleSceneRsv;
		//else if (i == 4)  auxevent = OrganPuzzleSceneRsv;
		roomEvent[auxevent]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		if (aux) entityManager->setActive(characterCorpse, true);
	}
}


void roomprueba::unload()
{
}
