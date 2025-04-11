#include "Room2.h"
#include "DialogueManager.h"
#include "ScrollComponent.h"
#include "ClickComponent.h"
#include "PauseManager.h"
#include "Image.h"
#include "Game.h"
#include "Log.h"
Room2Scene::Room2Scene()
{
	//Creation of the DialogueManager of the room and creation of the events 
	dialogueManager = new DialogueManager(1);
	_setRoomEvents();
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
		stopAnimation = false;
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
		rmObjects._quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(rmObjects._quitButton)->connect(ClickComponent::JUST_CLICKED, [this ,buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
			entityManager->setActive(rmObjects._quitButton, false);
			entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			if (!puzzlesol[2]) {
				rmObjects.mirror->getMngr()->setActive(rmObjects.mirror, false);
				rmObjects.secretEntry->getMngr()->setActive(rmObjects.secretEntry, false);
			}
		});
		entityManager->setActive(rmObjects._quitButton, false);

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

		Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);

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

		rmObjects.zoomCorpse = entityFactory->CreateImageEntity(entityManager, "patrisio",Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		rmObjects.zoomCorpse->getMngr()->setActive(rmObjects.zoomCorpse, false);

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
		rmObjects.organMosaic = entityFactory->CreateInteractableEntity(entityManager, "Organo", EntityFactory::RECTAREA, Vector2D(500 + 1344, 390), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.organMosaic));
		entityManager->getComponent<ClickComponent>(rmObjects.organMosaic)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[OrganZoom]();
		});
		rmObjects.organMosaic->getMngr()->setActive(rmObjects.organMosaic, false);

		//Zoomed Items

		rmObjects.zoomOrgan = entityFactory->CreateImageEntity(entityManager, "FondoOrgano", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		rmObjects.zoomOrgan->getMngr()->setActive(rmObjects.zoomOrgan, false);

		rmObjects.organ = entityFactory->CreateInteractableEntity(entityManager, "Organo", EntityFactory::RECTAREA, Vector2D(600, 350), Vector2D(0, 0), 500, 400, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(rmObjects.organ)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[OrganPuzzleScene]();
			roomEvent[OrganPuzzleSceneRsv](); //ONLY FOR DEBUGGING
		});
		rmObjects.organ->getMngr()->setActive(rmObjects.organ, false);

		rmObjects.rope = entityFactory->CreateInteractableEntity(entityManager, "Palo", EntityFactory::RECTAREA, Vector2D(200, 350), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(rmObjects.rope)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[Rope]();
		});
		rmObjects.rope->getMngr()->setActive(rmObjects.rope, false);

#pragma endregion

#pragma region SolvedOrgan

		//After completeing organ

		rmObjects.hook = entityFactory->CreateInteractableEntity(entityManager, "Gancho", EntityFactory::RECTAREA, Vector2D(400, 190), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(rmObjects.hook)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[Hook]();
			});
		rmObjects.hook->getMngr()->setActive(rmObjects.hook, false);

		rmObjects.mirror = entityFactory->CreateImageEntity(entityManager, "Ventana", Vector2D(400, 300), Vector2D(0, 0), 200 / 3, 235 / 3, 0, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.mirror));
		rmObjects.mirror->getMngr()->setActive(rmObjects.mirror, false);

		rmObjects.secretEntry = entityFactory->CreateInteractableEntity(entityManager, "PaasadizoSecretoEspejo", EntityFactory::RECTAREA, Vector2D(400, 390), Vector2D(0, 0), 500 / 3, 500 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.secretEntry));
		entityManager->getComponent<ClickComponent>(rmObjects.secretEntry)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[SecretEntry]();
		});
		rmObjects.secretEntry->getMngr()->setActive(rmObjects.secretEntry, false);


		rmObjects.secretEntryZoom = entityFactory->CreateImageEntity(entityManager, "EntradaOculta", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		rmObjects.secretEntryZoom->getMngr()->setActive(rmObjects.secretEntryZoom, false);

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


void Room2Scene::unload()
{
	entityManager->~EntityManager();
}

void Room2Scene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);

	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);

	if (finishallpuzzles)
	{
		roomEvent[ResolveButtons]();
	}
}

void Room2Scene::_setRoomEvents()
{
	roomEvent.resize(event_size);
#pragma region Events
	roomEvent[InitialDialogue] = [this] {
		startDialogue("SalaIntermedia2");
		};
	roomEvent[CorpseDialogue] = [this] {
		rmObjects.zoomCorpse->getMngr()->setActive(rmObjects.zoomCorpse, true);
		rmObjects._quitButton->getMngr()->setActive(rmObjects._quitButton, true);
		startDialogue("Cadaver");
		};
	roomEvent[TombPuzzleScene] = [this] {
		std::cout << "Se supone que entro en una escena btw" << std::endl;
		//Game::Instance()->getSceneManager()->loadScene(TOMB_SCENE, this); IMPORTANT: Include Tomb Scene when its finished
		};
	roomEvent[TombPuzzleSceneRsv] = [this] {


		};
	roomEvent[RavenScene] = [this] {
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
	roomEvent[MosaicPuzzleScene] = [this] {
		HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
		rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
		Game::Instance()->getSceneManager()->loadScene(MOSAIC_SCENE, this);
		};
	roomEvent[MosaicPuzzleSceneRsv] = [this] {
		rmObjects.organMosaic->getMngr()->setActive(rmObjects.organMosaic, true);
		};
	roomEvent[OrganZoom] = [this] {
		if (!puzzlesol[2]) rmObjects.hook->getMngr()->setActive(rmObjects.hook, false); //if organ is not complete don't show the hook
		rmObjects.zoomOrgan->getMngr()->setActive(rmObjects.zoomOrgan, true);
		rmObjects._quitButton->getMngr()->setActive(rmObjects._quitButton, true);
		rmObjects.organ->getMngr()->setActive(rmObjects.organ, true);
		if (rmObjects.rope != nullptr) rmObjects.rope->getMngr()->setActive(rmObjects.rope, true);
		};
	roomEvent[Rope] = [this] {
		//Remove rope
		rmObjects.rope->getMngr()->removeEntity(rmObjects.rope);
		rmObjects.rope->getMngr()->setActive(rmObjects.rope, false);
		// InventoryLogic
		GetInventory()->addItem(new Hint("Palo", "Ta", &sdlutils().images().at("Palo")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "Palo", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};
	roomEvent[WindowScene] = [this] {
		Game::Instance()->getSceneManager()->loadScene(WINDOW_SCENE, this);
		};
	roomEvent[OrganPuzzleScene] = [this] {
		//	Game::Instance()->getSceneManager()->loadScene(MUSIC_PUZZLE, this);
		};
	roomEvent[OrganPuzzleSceneRsv] = [this] {

		rmObjects.hook->getMngr()->setActive(rmObjects.hook, true);
		rmObjects.mirror->getMngr()->setActive(rmObjects.mirror, true);
		rmObjects.secretEntry->getMngr()->setActive(rmObjects.secretEntry, true);
		};
	roomEvent[Hook] = [this] {
		//Remove Hook
		rmObjects.hook->getMngr()->removeEntity(rmObjects.hook);
		// InventoryLogic
		GetInventory()->addItem(new Hint("Gancho", "Esto se enganchara en algun lado", &sdlutils().images().at("Gancho")));
		GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "Gancho", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);

		};
	roomEvent[SecretEntry] = [this] {
		//Depende de si es escena aparte o no
		rmObjects.secretEntryZoom->getMngr()->setActive(rmObjects.secretEntryZoom, true);
		rmObjects._quitButton->getMngr()->setActive(rmObjects._quitButton, true);
		};
	roomEvent[ResolveCase] = [this] {
		//IMPORTANT assign dialogue
		startDialogue("SalaFinal2");
		roomEvent[ResolveButtons]();
		// cahnge image every 1 sec
		SDL_AddTimer(1000, [](Uint32, void* param) -> Uint32 {
			auto* self = static_cast<decltype(this)>(param);
			if (!self) return 0;

			// if stopped animation change to normal botton
			if (self->stopAnimation) {
				Image* img = self->entityManager->getComponent<Image>(self->rmObjects.readyToResolveBotton);
				if (img) {
					img->setTexture(&sdlutils().images().at("B5"));
				}
				return 0;  // stop timer
			}

			// alternate between images
			static bool toggle = false;
			toggle = !toggle;
			const std::string& textureId = toggle ? "5.2" : "B5";

			// chnage botton texture
			if (auto* img = self->entityManager->getComponent<Image>(self->rmObjects.readyToResolveBotton)) {
				img->setTexture(&sdlutils().images().at(textureId));
			}

			// call timer again
			return 1000;
			}, this);
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

void Room2Scene::_setRoomAudio()
{
	//Audio sfx 
	AudioManager& audioMngr = AudioManager::Instance();

	rmSounds.uiButton = sdlutils().soundEffects().at("boton");
	audioMngr.setVolume(rmSounds.uiButton, 0.2);

	rmSounds.puzzleButton = sdlutils().soundEffects().at("puzzle");
	audioMngr.setVolume(rmSounds.puzzleButton, 0.3);

	rmSounds.doorSound = sdlutils().soundEffects().at("puerta");


	/*Audio music
	Sound room1music = sdlutils().musics().at("sala2");
	audioMngr.setLooping(room1music, true);
	audioMngr.playSound(room1music);*/
}

void Room2Scene::_setGlobalFeatures()
{
	//Register scene in dialogue manager
	dialogueManager->setScene(this);
}

void Room2Scene::_setRoomBackground()
{
}

void Room2Scene::_setCaseResolution()
{
	//set the scene the variant is 
	Game::Instance()->getDataManager()->SetSceneCount(ROOM1);

	//get actual variant
	int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(0);


	auto background = entityFactory->CreateImageEntity(
		entityManager, "fondoPruebaLog", Vector2D(2, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);

	entityManager->setActive(background, false);

	entityManager->addComponent<RectArea2D>(background, areaLayerManager);
	auto backgroundRect = entityManager->getComponent<RectArea2D>(background);
	areaLayerManager->sendFront(backgroundRect->getLayerPos());

	auto readyToQuestion = entityFactory->CreateImageEntity(entityManager, "1stQuestion", Vector2D(350, 200), Vector2D(0, 0), 600, 200, 0, ecs::grp::DECISION);
	entityManager->setActive(readyToQuestion, false);

	entityManager->addComponent<RectArea2D>(readyToQuestion, areaLayerManager);
	auto readyToQuestionRect = entityManager->getComponent<RectArea2D>(readyToQuestion);
	areaLayerManager->sendFront(readyToQuestionRect->getLayerPos());

	auto finalQuestion = entityFactory->CreateImageEntity(entityManager, "2ndQuestion", Vector2D(350, 200), Vector2D(0, 0), 600, 200, 0, ecs::grp::DECISION);
	entityManager->setActive(finalQuestion, false);

	entityManager->addComponent<RectArea2D>(finalQuestion, areaLayerManager);
	auto finalQuestionRect = entityManager->getComponent<RectArea2D>(finalQuestion);
	areaLayerManager->sendFront(finalQuestionRect->getLayerPos());

	auto possibleButton = entityFactory->CreateInteractableEntity(entityManager, "yes", EntityFactory::RECTAREA, Vector2D(400, 420), Vector2D(0, 0), 200, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DECISION);
	entityManager->setActive(possibleButton, false);

	entityManager->getComponent<ClickComponent>(possibleButton)
		->connect(ClickComponent::JUST_CLICKED, [this, variantAct, background]()
			{
				if (variantAct != 0) //if its the not correct variant one dies
				{

					Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
					roomEvent[BadEnd]();
				}
				else
				{
					roomEvent[GoodEnd]();
				}

				entityManager->setActive(background, false);

			});

	auto possibleButtonRect = entityManager->getComponent<RectArea2D>(possibleButton);
	areaLayerManager->sendFront(possibleButtonRect->getLayerPos());


	auto noPossibleButton = entityFactory->CreateInteractableEntity(entityManager, "no", EntityFactory::RECTAREA, Vector2D(700, 420), Vector2D(0, 0), 200, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DECISION);
	entityManager->setActive(noPossibleButton, false);

	entityManager->getComponent<ClickComponent>(noPossibleButton)
		->connect(ClickComponent::JUST_CLICKED, [this, variantAct, background]()
			{
				if (variantAct == 0) //if its the not correct variant one dies
				{

					Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
					roomEvent[BadEnd]();

				}
				else
				{
					roomEvent[GoodEnd]();
				}
				entityManager->setActive(background, false);

			});

	auto noPossibleButtonRect = entityManager->getComponent<RectArea2D>(noPossibleButton);
	areaLayerManager->sendFront(noPossibleButtonRect->getLayerPos());

	auto resolveButton = entityFactory->CreateInteractableEntity(entityManager, "yes", EntityFactory::RECTAREA, Vector2D(400, 420), Vector2D(0, 0), 200, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DECISION);
	entityManager->setActive(resolveButton, false);

	auto resolveButtonRect = entityManager->getComponent<RectArea2D>(resolveButton);
	areaLayerManager->sendFront(resolveButtonRect->getLayerPos());

	auto noResolveButton = entityFactory->CreateInteractableEntity(entityManager, "no", EntityFactory::RECTAREA, Vector2D(700, 420), Vector2D(0, 0), 200, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DECISION);
	entityManager->setActive(noResolveButton, false);

	auto noResolveButtonRect = entityManager->getComponent<RectArea2D>(noResolveButton);
	areaLayerManager->sendFront(noResolveButtonRect->getLayerPos());


	//Button only appears when the 3 puzzles have been resolved
	rmObjects.readyToResolveBotton = entityFactory->CreateInteractableEntity(entityManager, "B5", EntityFactory::RECTAREA, Vector2D(190 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

	entityManager->getComponent<ClickComponent>(rmObjects.readyToResolveBotton)
		->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToQuestion, background]()
			{
				stopAnimation = true;
				entityManager->setActive(noResolveButton, true);
				entityManager->setActive(resolveButton, true);
				entityManager->setActive(readyToQuestion, true);
				entityManager->setActive(rmObjects.readyToResolveBotton, false);
				entityManager->setActive(background, true);

			});

	entityManager->setActive(rmObjects.readyToResolveBotton, false);

	entityManager->getComponent<ClickComponent>(resolveButton)
		->connect(ClickComponent::JUST_CLICKED, [this, resolveButton, noPossibleButton, possibleButton, noResolveButton, readyToQuestion, finalQuestion]()
			{
				entityManager->setActive(noPossibleButton, true);
				entityManager->setActive(readyToQuestion, false);
				entityManager->setActive(rmObjects.readyToResolveBotton, false);
				entityManager->setActive(possibleButton, true);
				entityManager->setActive(finalQuestion, true);
				entityManager->setActive(resolveButton, false);
				entityManager->setActive(noResolveButton, false);
			});

	entityManager->getComponent<ClickComponent>(noResolveButton)
		->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToQuestion, background]()
			{
				entityManager->setActive(readyToQuestion, false);
				entityManager->setActive(noResolveButton, false);
				entityManager->setActive(background, false);
				entityManager->setActive(resolveButton, false);
				entityManager->setActive(rmObjects.readyToResolveBotton, true);
			});

	rmObjects.blackBackground = entityFactory->CreateImageEntity(
		entityManager, "FondoNegro", Vector2D(2, 0), Vector2D(0, 0), 1356, 758, 0, ecs::grp::DECISION);
	entityManager->setActive(rmObjects.blackBackground, false);


}

void Room2Scene::_setInteractuables()
{
}

void Room2Scene::_setDialog()
{
	// Dialog
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, _eventToRead);

	assert(rmObjects.inventoryButton != nullptr); // UI must be Initialized First

	Area2D* inventoryButtonArea = entityManager->getComponent<Area2D>(rmObjects.inventoryButton);

	auto dialogEnts = entityManager->getEntities(ecs::grp::DIALOGUE);

	for (ecs::entity_t dialogEnt : dialogEnts)
	{
		Area2D* dialogArea = entityManager->getComponent<Area2D>(dialogEnt);
		if (dialogArea != nullptr)
			areaLayerManager->sendAfter(inventoryButtonArea->getLayerPos(), dialogArea->getLayerPos());
	}
}

void Room2Scene::_setUI()
{
#pragma region QuitButton
	// Corpse zoom Quit Button
	rmObjects._quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);

	entityManager->getComponent<ClickComponent>(rmObjects._quitButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				auto ImagequitButton = entityManager->getComponent<Image>(rmObjects._quitButton);
				ImagequitButton->setW(entityManager->getComponent<Transform>(rmObjects._quitButton)->getWidth());
				ImagequitButton->setH(entityManager->getComponent<Transform>(rmObjects._quitButton)->getHeight());
				ImagequitButton->setPosOffset(0, 0);
				AudioManager::Instance().playSound(rmSounds.uiButton);
				entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
				entityManager->setActive(rmObjects._quitButton, false);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			});

	entityManager->setActive(rmObjects._quitButton, false);
#pragma endregion

#pragma region Inventory
	//inventory descriptions

//visual background for item description text
	invObjects.backgroundTextDescription = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(150, 800), Vector2D(0, 0), 500, 75, 0, ecs::grp::UI);
	entityManager->setActive(invObjects.backgroundTextDescription, false);

	//description text entity
	invObjects.textDescriptionEnt = entityManager->addEntity(ecs::grp::UI);
	auto _testTextTranform = entityManager->addComponent<Transform>(invObjects.textDescriptionEnt, Vector2D(600, 300), Vector2D(0, 0), 300, 200, 0);
	entityManager->setActive(invObjects.textDescriptionEnt, false);
	SDL_Color colorDialog = { 255, 255, 255, 255 };
	entityManager->addComponent<WriteTextComponent<DescriptionInfo>>(invObjects.textDescriptionEnt, sdlutils().fonts().at("BASE"), colorDialog, GetInventory()->getTextDescription());


	//Inventory

	invObjects.InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);

	rmObjects.inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.InventoryBackground, false);

	invObjects.InvArea = entityManager->addComponent<RectArea2D>(invObjects.InventoryBackground, areaLayerManager);

	invObjects.inventoryUpButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.inventoryUpButton, false);

	invObjects.inventoryDownButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.inventoryDownButton, false);

	entityManager->getComponent<ClickComponent>(rmObjects.inventoryButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

				if (GetInventory()->getActive()) // If the inventory is active, activate the items
				{
					entityManager->setActive(invObjects.InventoryBackground, true);
					entityManager->setActive(rmObjects.logbtn, false);
					//change the position of the log button
					areaLayerManager->sendFront(invObjects.InvArea->getLayerPos());
					entityManager->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(925);

					areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryUpButton)->getLayerPos());
					areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryDownButton)->getLayerPos());

					entityManager->setActive(invObjects.inventoryDownButton, true);
					entityManager->setActive(invObjects.inventoryUpButton, true);

					for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber() + GetInventory()->getFirstItem(); ++i) GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
				}
				else
				{
					entityManager->setActive(invObjects.InventoryBackground, false);
					entityManager->setActive(invObjects.inventoryDownButton, false);
					entityManager->setActive(invObjects.inventoryUpButton, false);
					entityManager->setActive(rmObjects.logbtn, true);
					rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
					for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber() + GetInventory()->getFirstItem(); ++i) GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Activate the items

				}
			});

	entityManager->getComponent<ClickComponent>(invObjects.inventoryUpButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				scrollInventory(-1);
			});
	entityManager->getComponent<ClickComponent>(invObjects.inventoryDownButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				scrollInventory(1);
			});

	entityManager->setActive(rmObjects._quitButton, false);
#pragma endregion

#pragma region Pause
	// Pause Logic
	pauseManager->setScene(this);
	pauseManager->Init(entityFactory, entityManager, areaLayerManager);

	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getbackgroundNotInteractable())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getreanudePauseButton())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getexitPauseButton())->getLayerPos());

	rmObjects.logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager, this);
#pragma endregion

}
