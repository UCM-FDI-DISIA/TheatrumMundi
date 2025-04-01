#include "Room2.h"
#include "DialogueManager.h"
#include "ScrollComponent.h"
#include "ClickComponent.h"
#include "Game.h"
Room2Scene::Room2Scene()
{
	//Creation of the DialogueManager of the room and creation of the events 
	dialogueManager = new DialogueManager(1);
	roomEvent.resize(event_size);
#pragma region Events
	roomEvent[InitialDialogue] = [this]{
		startDialogue("SalaIntermedia2");
	};
	roomEvent[CorpseDialogue] = [this]{
			roomEvent[ResolveCase]();
			startDialogue("Cadaver");
	};
	roomEvent[TombPuzzleScene] = [this]{
		std::cout << "Se supone que entro en una escena btw" << std::endl;
		//Game::Instance()->getSceneManager()->loadScene(TOMB_SCENE, this); IMPORTANT: Include Tomb Scene when its finished
	};
	roomEvent[TombPuzzleSceneRsv] = [this] {
		

	};
	roomEvent[RavenScene] = [this]{
		std::cout << "Cuervo cuervito cuervón" << std::endl;
		//Game::Instance()->getSceneManager()->loadScene(RAVEN_SCENE, this); IMPORTANT: Include Raven Scene when its finished
	};
	roomEvent[RavenSceneRsv] = [this] {

	};
	roomEvent[DoorScene] = [this] {
		std::cout << "Puerta!!!!!" << std::endl;
		//Game::Instance()->getSceneManager()->loadScene(DOOR_SCENE, this); IMPORTANT: Include Door Scene when its finished
	};
	roomEvent[DoorSceneRsv] = [this] {
		std::cout << "Abrete Sésamo" << std::endl;
		isOpen = true;
	};
	roomEvent[MosaicPuzzleScene] = [this]{
			Game::Instance()->getSceneManager()->loadScene(MOSAIC_SCENE, this);
	};
	roomEvent[MosaicPuzzleSceneRsv] = [this] {
		organ->getMngr()->setActive(organ, true);
	};
	roomEvent[Stick] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("WindowStick", "Parece que se puede abrir la ventana con esto", &sdlutils().images().at("WindowStick")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "WindowStick", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
	};
	roomEvent[WindowScene] = [this] {
		//Game::Instance()->getSceneManager()->loadScene(WINDOW_SCENE, this); IMPORTANT: Include Window Scene when its finished
	};
	roomEvent[OrganPuzzleScene] = [this] {
		//Game::Instance()->getSceneManager()->loadScene(ORGAN_SCENE, this); IMPORTANT: Include Organ Scene when its finished
	};
	roomEvent[OrganPuzzleSceneRsv] = [this] {
		mirror->getMngr()->setActive(mirror, true);
		secretEntry->getMngr()->setActive(secretEntry, true);
	};
	roomEvent[Hook] = [this] {
		// InventoryLogic
		GetInventory()->addItem(new Hint("Hook", "Esto se enganchara en algun lado", &sdlutils().images().at("Hook")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "Hook", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
	};
	roomEvent[SecretExit] = [this] {
		//Depende de si es escena aparte o no
	};
	roomEvent[ResolveCase] = [this] {
		//IMPORTANT assign dialogue
		startDialogue("SalaIntermedia1");
	};
	roomEvent[ResolveButtons] = [this] {
		//Activar los botones o inclusive crearlos aquí directamente dependiendo de si puedes salir o no
	};
	roomEvent[GoodEnd] = [this] {
		// WIP
		Game::Instance()->getSceneManager()->popScene();
	};
	roomEvent[BadEnd] = [this] {
		// WIP
		Game::Instance()->getSceneManager()->popScene();
	};
#pragma endregion
}

Room2Scene::~Room2Scene()
{
}

void Room2Scene::init()
{
	if (!isStarted) {

		isStarted = true;
		isOpen = false;
		finishallpuzzles = false;
		dialogueManager->setScene(this);
		Game::Instance()->getDataManager()->SetSceneCount(ROOM2);
		int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(2);

#pragma region Audio
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);
		Sound puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		a.setVolume(puzzleButtonSound, 0.3);
		Sound doorSound = sdlutils().soundEffects().at("puerta");
		//Audio music
		//Sound room2music = sdlutils().musics().at("room2music");
		//a.setLooping(room2music, true);
		//a.playSound(room2music);
#pragma endregion

#pragma region InitScroll
		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1200, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto CementeryBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom2);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom1));
		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
#pragma endregion

#pragma region UI
		//Quit
		auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(_quitButton)->connect(ClickComponent::JUST_CLICKED, [this, _quitButton ,buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);
				entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
				entityManager->setActive(_quitButton, false);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			});
		entityManager->setActive(_quitButton, false);

		//Pause
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});

		//Inventory
		auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(0, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
		auto buttonInventory = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(InventoryBackground, false);

		auto upButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(upButton, false);

		auto downButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->setActive(downButton, false);

		ClickComponent* buttonInventoryClick = entityManager->getComponent<ClickComponent>(buttonInventory);
		buttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, InventoryBackground, upButton, downButton, buttonInventory]() {
			AudioManager::Instance().playSound(buttonSound);
			GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

			// If the inventory is active, activate the items
			if (GetInventory()->getActive()) {
				entityManager->setActive(InventoryBackground, true);

				buttonInventory->getMngr()->getComponent<Transform>(buttonInventory)->getPos().setX(20);
				entityManager->setActive(downButton, true);
				entityManager->setActive(upButton, true);

				for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i) {
					GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
				}
			}
			else {
				entityManager->setActive(InventoryBackground, false);
				entityManager->setActive(downButton, false);
				entityManager->setActive(upButton, false);
				buttonInventory->getMngr()->getComponent<Transform>(buttonInventory)->getPos().setX(60 + 268 / 3);

				// its okay to use the first item as the first item to show??
				for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i) {
					GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Desactivate the hints
				}
			}

		});


		ClickComponent* DOWNbuttonInventoryClick = entityManager->getComponent<ClickComponent>(downButton);
		DOWNbuttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, downButton]() {
			AudioManager::Instance().playSound(buttonSound);
			scrollInventory(1);
		});
#pragma endregion

#pragma region Background

		//LeftBackground
		auto CementeryBackground = entityFactory->CreateImageEntity(entityManager, "FondoCementerio", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(CementeryBackground));

		//RightBackground
		auto MausoleumBackground = entityFactory->CreateImageEntity(entityManager, "FondoCripta", Vector2D(1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(MausoleumBackground));


#pragma endregion

#pragma region Scroll

		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, CementeryBackgroundScroll, doorSound]() {
			if (isOpen && !CementeryBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				CementeryBackgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
		});

		ChangeRoom2Button->connect(ClickComponent::JUST_CLICKED, [this, CementeryBackgroundScroll, doorSound]() {
			if (isOpen && !CementeryBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				CementeryBackgroundScroll->Scroll(ScrollComponent::LEFT);
			}
		});
#pragma endregion

#pragma region Answer
		// Button that confirms the assesination
		auto buttonPosible = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3) - 20), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(buttonPosible)->connect(ClickComponent::JUST_CLICKED, [this]() { roomEvent[GoodEnd](); });
		entityManager->setActive(buttonPosible, false);

		// Button that confirms the assesination is not possible
		auto buttonImposible = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3)), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(buttonImposible)->connect(ClickComponent::JUST_CLICKED, [this]() { roomEvent[BadEnd](); });
		entityManager->setActive(buttonImposible, false);

		//Resolve the case
		roomEvent[ResolveButtons] = [this, buttonPosible, buttonImposible]() {
			entityManager->setActive(buttonPosible, true);
			entityManager->setActive(buttonImposible, true);
	};


#pragma endregion

#pragma region CementeryEntities
		auto tomb = entityFactory->CreateInteractableEntity(entityManager, "Tumba", EntityFactory::RECTAREA, Vector2D(400, 300), Vector2D(0, 0), 250 / 3, 225 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(tomb));
		entityManager->getComponent<ClickComponent>(tomb)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[TombPuzzleScene]();
			resolvedPuzzle(4);
		});
		auto raven = entityFactory->CreateInteractableEntity(entityManager, "Cuervo", EntityFactory::RECTAREA, Vector2D(700, 452), Vector2D(0, 0), 165 / 3, 176 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(raven));
		entityManager->getComponent<ClickComponent>(raven)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[RavenScene]();
			resolvedPuzzle(3);
		});
		auto door = entityFactory->CreateInteractableEntity(entityManager, "PuertaSantuario", EntityFactory::RECTAREA, Vector2D(1200, 300), Vector2D(0, 0), 450 / 3, 626 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(door));
		entityManager->getComponent<ClickComponent>(door)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[DoorScene]();
			resolvedPuzzle(2);
		});
#pragma endregion

#pragma region MausoleumEntities

		//Before Completing mosaic
		auto mosaic = entityFactory->CreateInteractableEntity(entityManager, "Mosaico", EntityFactory::RECTAREA, Vector2D(400 + 1344, 300), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(mosaic));
		entityManager->getComponent<ClickComponent>(mosaic)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[MosaicPuzzleScene]();
		});
		auto window = entityFactory->CreateInteractableEntity(entityManager, "Ventana", EntityFactory::RECTAREA, Vector2D(1200 + 1344, 452), Vector2D(0, 0), 300 / 3, 276 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(window));
		entityManager->getComponent<ClickComponent>(window)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[WindowScene]();
		});

		//After completeing mosaic
		organ = entityFactory->CreateInteractableEntity(entityManager, "Organo", EntityFactory::RECTAREA, Vector2D(500 + 1344, 390), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(organ));
		entityManager->getComponent<ClickComponent>(organ)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[OrganPuzzleScene]();
		});
		organ->getMngr()->setActive(organ, false);

		//After completeing organ
		mirror = entityFactory->CreateImageEntity(entityManager, "Ventana",Vector2D(400 + 1344, 300), Vector2D(0, 0), 200 / 3, 235 / 3, 0, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(mirror));
		mirror->getMngr()->setActive(mirror, false);

		secretEntry = entityFactory->CreateInteractableEntity(entityManager, "PaasadizoSecretoEspejo", EntityFactory::RECTAREA, Vector2D(400 + 1344, 390), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(secretEntry));
		entityManager->getComponent<ClickComponent>(secretEntry)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[SecretExit]();
		});
		secretEntry->getMngr()->setActive(secretEntry, false);

#pragma endregion


	}
}

void Room2Scene::resolvedPuzzle(int i)
{
	if (i < 5) {
		int auxevent = event_size;
		if (i == 0)  auxevent = TombPuzzleSceneRsv;
		else if (i == 1)  auxevent = RavenSceneRsv;
		else if (i == 2)  auxevent = DoorSceneRsv;
		else if (i == 3)  auxevent = MosaicPuzzleSceneRsv;
		else if (i == 4)  auxevent = OrganPuzzleSceneRsv;
		roomEvent[auxevent]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		if (aux) entityManager->setActive(body, true);
	}
}

void Room2Scene::refresh()
{
}

void Room2Scene::unload()
{
}
