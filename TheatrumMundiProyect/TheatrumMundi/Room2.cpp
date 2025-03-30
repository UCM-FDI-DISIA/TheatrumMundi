#include "Room2.h"
#include "DialogueManager.h"
#include "ScrollComponent.h"
#include "ClickComponent.h"
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
			//Game::Instance()->getSceneManager()->loadScene(TOMB_SCENE, this); IMPORTANT: Include Tomb Scene when its finished
	};
	roomEvent[TombPuzzleSceneRsv] = [this] {
		

	};
	roomEvent[RavenScene] = [this]{
			//Game::Instance()->getSceneManager()->loadScene(RAVEN_SCENE, this); IMPORTANT: Include Raven Scene when its finished
	};
	roomEvent[RavenSceneRsv] = [this] {

	};
	roomEvent[DoorScene] = [this] {
			//Game::Instance()->getSceneManager()->loadScene(DOOR_SCENE, this); IMPORTANT: Include Door Scene when its finished
	};
	roomEvent[DoorSceneRsv] = [this] {
		isOpen = true;
	};
	roomEvent[MosaicPuzzleScene] = [this]{
			Game::Instance()->getSceneManager()->loadScene(MOSAIC_SCENE, this);
	};
	roomEvent[MosaicPuzzleSceneRsv] = [this] {

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
		//Activación del espejo para hacer el zoom al clickar 
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
		int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(ROOM1);

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
		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1160 - 1349, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));
		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
#pragma endregion

#pragma region UI
		auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(_quitButton)->connect(ClickComponent::JUST_CLICKED, [this, _quitButton ,buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);
				entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
				entityManager->setActive(_quitButton, false);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			});
		entityManager->setActive(_quitButton, false);
#pragma endregion

#pragma region Background

		//LeftBackground
		auto CementeryBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		auto CementeryBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(CementeryBackground));

		//RightBackground
		auto MausoleumBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(-1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		CementeryBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(MausoleumBackground));


#pragma endregion

#pragma region Scroll

		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom1Button, CementeryBackgroundScroll, doorSound, ChangeRoomScroll]() {
			if (isOpen && !CementeryBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				CementeryBackgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
		});

		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		ChangeRoom2Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom2Button, CementeryBackgroundScroll, doorSound]() {
			if (isOpen && !CementeryBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				CementeryBackgroundScroll->Scroll(ScrollComponent::LEFT);
			}
		});
#pragma endregion

#pragma region Answer

		auto possibleButton = entityFactory->CreateInteractableEntity(entityManager, "posible", EntityFactory::RECTAREA, Vector2D(500, 0), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(possibleButton, false);
		entityManager->getComponent<ClickComponent>(possibleButton)->connect(ClickComponent::JUST_CLICKED, [this, variantAct]() {
			//if its the not correct variant one dies
			if (variantAct != 0) Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
		});

		auto noPossibleButton = entityFactory->CreateInteractableEntity(entityManager, "noPosible", EntityFactory::RECTAREA, Vector2D(600, 0), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(noPossibleButton, false);
		entityManager->getComponent<ClickComponent>(noPossibleButton)->connect(ClickComponent::JUST_CLICKED, [this, variantAct]() {
			//if its the not correct variant one dies
			if (variantAct != 1) Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
		});

		auto resolveButton = entityFactory->CreateInteractableEntity(entityManager, "resolve", EntityFactory::RECTAREA, Vector2D(0, 500), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(resolveButton, false);

		auto noResolveButton = entityFactory->CreateInteractableEntity(entityManager, "noResolve", EntityFactory::RECTAREA, Vector2D(0, 600), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->setActive(noResolveButton, false);

		//appears when the 3 puzzles have been resolved
		auto readyToResolveButton = entityFactory->CreateInteractableEntity(entityManager, "B5", EntityFactory::RECTAREA, Vector2D(400, 400), Vector2D(0, 0), 400, 400, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		entityManager->getComponent<ClickComponent>(readyToResolveButton)->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToResolveButton]() {
			//Shows to the player if they want to finished the room
			entityManager->setActive(noResolveButton, true);
			entityManager->setActive(resolveButton, true);
			entityManager->setActive(readyToResolveButton, false);
		});
		entityManager->setActive(readyToResolveButton, false);
		entityManager->setActive(readyToResolveButton, false);


		entityManager->getComponent<ClickComponent>(resolveButton)->connect(ClickComponent::JUST_CLICKED, [this, possibleButton, noPossibleButton, readyToResolveButton, resolveButton, noResolveButton]() {
			//Shows and hide the answer buttons
			entityManager->setActive(noPossibleButton, true);
			entityManager->setActive(readyToResolveButton, false);
			entityManager->setActive(possibleButton, true);
			entityManager->setActive(resolveButton, false);
			entityManager->setActive(noResolveButton, false);
		});

		entityManager->getComponent<ClickComponent>(noResolveButton)->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToResolveButton]() {
			//Returns player to the scene
			entityManager->setActive(noResolveButton, false);
			entityManager->setActive(resolveButton, false);
			entityManager->setActive(readyToResolveButton, true);
		});

#pragma endregion

	}
}

void Room2Scene::resolvedPuzzle(int i)
{
}

void Room2Scene::refresh()
{
}

void Room2Scene::unload()
{
}
