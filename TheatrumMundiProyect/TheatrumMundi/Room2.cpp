#include "Room2.h"
#include "DialogueManager.h"
#include "ScrollComponent.h"
#include "ClickComponent.h"
#include "Game.h"
#include "Log.h"
#include "RectArea2D.h"
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
		zoomCorpse->getMngr()->setActive(zoomCorpse, true);
		_quitButton->getMngr()->setActive(_quitButton, true);
		startDialogue("Cadaver");
	};
	roomEvent[TombPuzzleScene] = [this]{
		std::cout << "Se supone que entro en una escena btw" << std::endl;
		//Game::Instance()->getSceneManager()->loadScene(TOMB_SCENE, this); IMPORTANT: Include Tomb Scene when its finished
	};
	roomEvent[TombPuzzleSceneRsv] = [this] {
		

	};
	roomEvent[RavenScene] = [this]{
		Game::Instance()->getSceneManager()->loadScene(RAVEN_SCENE, this);
	};
	roomEvent[RavenSceneRsv] = [this] {

	};
	roomEvent[DoorScene] = [this] {
		Game::Instance()->getSceneManager()->loadScene(DOOR_SCENE, this);
	};
	roomEvent[DoorSceneRsv] = [this] {
		isOpen = true;
	};
	roomEvent[MosaciZoom] = [this] {

	};
	roomEvent[MosaicPuzzleScene] = [this]{
		Game::Instance()->getSceneManager()->loadScene(MOSAIC_SCENE, this);
	};
	roomEvent[MosaicPuzzleSceneRsv] = [this] {
		organMosaic->getMngr()->setActive(organMosaic, true);
	};
	roomEvent[OrganZoom] = [this] {
		if (!puzzlesol[2]) hook->getMngr()->setActive(hook, false); //if organ is not complete don't show the hook
		zoomOrgan->getMngr()->setActive(zoomOrgan, true);
		_quitButton->getMngr()->setActive(_quitButton, true);
		organ->getMngr()->setActive(organ, true);
		if(rope != nullptr) rope->getMngr()->setActive(rope, true);
	};
	roomEvent[Rope] = [this] {
		//Remove rope
		rope->getMngr()->removeEntity(this->rope);
		// InventoryLogic
		GetInventory()->addItem(new Hint("Palo", "Ta", &sdlutils().images().at("Palo")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "Palo", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
	};
	roomEvent[WindowScene] = [this] {
		//Game::Instance()->getSceneManager()->loadScene(WINDOW_SCENE, this); IMPORTANT: Include Window Scene when its finished
	};
	roomEvent[OrganPuzzleScene] = [this] {
		std::cout << "Tremenda trompeta digo organo" << std::endl;
		//Game::Instance()->getSceneManager()->loadScene(ORGAN_SCENE, this); IMPORTANT: Include Organ Scene when its finished
	};
	roomEvent[OrganPuzzleSceneRsv] = [this] {

		hook->getMngr()->setActive(hook, true);
		mirror->getMngr()->setActive(mirror, true);
		secretEntry->getMngr()->setActive(secretEntry, true);
	};
	roomEvent[Hook] = [this] {
		//Remove Hook
		hook->getMngr()->removeEntity(hook);
		// InventoryLogic
		GetInventory()->addItem(new Hint("Gancho", "Esto se enganchara en algun lado", &sdlutils().images().at("Gancho")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "Gancho", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		
	};
	roomEvent[SecretEntry] = [this] {
		//Depende de si es escena aparte o no
		secretEntryZoom->getMngr()->setActive(secretEntryZoom, true);
		_quitButton->getMngr()->setActive(_quitButton, true);
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
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(22, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto CementeryBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom2);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom1));
		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);

#pragma endregion

#pragma region UI

		//Quit
		_quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(_quitButton)->connect(ClickComponent::JUST_CLICKED, [this ,buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
			entityManager->setActive(_quitButton, false);
			entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			if (!puzzlesol[2]) {
				mirror->getMngr()->setActive(mirror, false);
				secretEntry->getMngr()->setActive(secretEntry, false);
			}
		});
		entityManager->setActive(_quitButton, false);

		//Pause
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
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
			->connect(ClickComponent::JUST_CLICKED, [this, InventoryBackground, inventoryUpButton, inventoryDownButton, InvArea,inventoryButton,buttonSound]()
				{
					AudioManager::Instance().playSound(buttonSound);
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

		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);

		//startDialogue("Room2");


#pragma endregion

#pragma region Background

		//LeftBackground
		auto CementeryBackground = entityFactory->CreateImageEntity(entityManager, "FondoCementerio", Vector2D(-10, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::DEFAULT);
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
			else if (!isOpen) {
				roomEvent[DoorScene]();
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

		auto corpse = entityFactory->CreateInteractableEntity(entityManager, "Cadaver2", EntityFactory::RECTAREA, Vector2D(100, 300), Vector2D(0, 0), 250 / 3, 225 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(corpse));
		entityManager->getComponent<ClickComponent>(corpse)->connect(ClickComponent::JUST_CLICKED, [this]() {
			if (!finishallpuzzles)roomEvent[CorpseDialogue]();
			else roomEvent[ResolveButtons]();
		});

		zoomCorpse = entityFactory->CreateImageEntity(entityManager, "patrisio",Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		zoomCorpse->getMngr()->setActive(zoomCorpse, false);

		auto tomb = entityFactory->CreateInteractableEntity(entityManager, "Tumba", EntityFactory::RECTAREA, Vector2D(400, 300), Vector2D(0, 0), 250 / 3, 225 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(tomb));
		entityManager->getComponent<ClickComponent>(tomb)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[TombPuzzleScene]();
		});
		
		auto raven = entityFactory->CreateInteractableEntity(entityManager, "Cuervo", EntityFactory::RECTAREA, Vector2D(700, 452), Vector2D(0, 0), 165 / 3, 176 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(raven));
		entityManager->getComponent<ClickComponent>(raven)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[RavenScene]();
		});

#pragma endregion

#pragma region MausoleumEntities

#pragma region PreSolveMosaic

		//Before Completing mosaic
		auto mosaic = entityFactory->CreateInteractableEntity(entityManager, "Mosaico", EntityFactory::RECTAREA, Vector2D(400 + 1344, 300), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(mosaic));
		entityManager->getComponent<ClickComponent>(mosaic)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[MosaicPuzzleScene]();
		});

		auto window = entityFactory->CreateInteractableEntity(entityManager, "Ventana", EntityFactory::RECTAREA, Vector2D(1250 + 1344, 150), Vector2D(0, 0), 400 / 3, 376 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(window));
		entityManager->getComponent<ClickComponent>(window)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[WindowScene]();
		});

#pragma endregion

#pragma region SolvedMosaic

		//After completeing mosaic
		organMosaic = entityFactory->CreateInteractableEntity(entityManager, "Organo", EntityFactory::RECTAREA, Vector2D(500 + 1344, 390), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(organMosaic));
		entityManager->getComponent<ClickComponent>(organMosaic)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[OrganZoom]();
		});
		organMosaic->getMngr()->setActive(organMosaic, false);

		//Zoomed Items

		zoomOrgan = entityFactory->CreateImageEntity(entityManager, "FondoOrgano", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		zoomOrgan->getMngr()->setActive(zoomOrgan, false);

		organ = entityFactory->CreateInteractableEntity(entityManager, "Organo", EntityFactory::RECTAREA, Vector2D(600, 350), Vector2D(0, 0), 500, 400, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(organ)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[OrganPuzzleScene]();
			resolvedPuzzle(4);
			puzzlesol[2] = true;
		});
		organ->getMngr()->setActive(organ, false);

		rope = entityFactory->CreateInteractableEntity(entityManager, "Palo", EntityFactory::RECTAREA, Vector2D(200, 350), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(rope)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[Rope]();
		});
		rope->getMngr()->setActive(rope, false);

#pragma endregion

#pragma region SolvedOrgan

		//After completeing organ

		hook = entityFactory->CreateInteractableEntity(entityManager, "Gancho", EntityFactory::RECTAREA, Vector2D(400, 190), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(hook)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[Hook]();
			});
		hook->getMngr()->setActive(hook, false);

		mirror = entityFactory->CreateImageEntity(entityManager, "Ventana", Vector2D(400, 300), Vector2D(0, 0), 200 / 3, 235 / 3, 0, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(mirror));
		mirror->getMngr()->setActive(mirror, false);

		secretEntry = entityFactory->CreateInteractableEntity(entityManager, "PaasadizoSecretoEspejo", EntityFactory::RECTAREA, Vector2D(400, 390), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(secretEntry));
		entityManager->getComponent<ClickComponent>(secretEntry)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[SecretEntry]();
		});
		secretEntry->getMngr()->setActive(secretEntry, false);


		secretEntryZoom = entityFactory->CreateImageEntity(entityManager, "EntradaOculta", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		secretEntryZoom->getMngr()->setActive(secretEntryZoom, false);

#pragma endregion
		
#pragma endregion


	}
	SDL_Delay(1000);

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
		if (aux) entityManager->setActive(characterCorpse, true);
	}
}

void Room2Scene::refresh()
{
}

void Room2Scene::unload()
{
}
