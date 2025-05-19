#include "Room2.h"
#include "DataManager.h"
#include "Vector2D.h"
#include "Transform.h"
#include "Image.h"
#include "SDLUtils.h"
#include "LogComponent.h"
#include "PhysicsBodyComponent.h"
#include "ClickComponent.h"
#include "TriggerComponent.h"
#include "DragComponent.h"
#include "CircleArea2D.h"
#include "RectArea2D.h"
#include "ScrollComponent.h"
#include "Manager.h"
#include "Game.h"
#include "ClickableSpriteComponent.h"
#include "EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"
#include "PauseManager.h"
#include "WriteTextComponent.h"
#include "DialogueManager.h"
#include "InvAnimComponent.h"
Room2Scene::Room2Scene()
{
	//Creation of the DialogueManager of the room and creation of the events 
	dialogueManager = new DialogueManager(3);
	_setRoomEvents();
}

Room2Scene::~Room2Scene()
{

}

void Room2Scene::init()
{
	if (isStarted) return;
	isStarted = true;
	isOpen = false;
	finishallpuzzles = false;
	stopAnimation = false;

	//Load Images
	_loadimg1 = entityFactory->CreateImageEntity(entityManager, "loading1", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg1->getMngr()->setActive(_loadimg1, false);

	_loadimg2 = entityFactory->CreateImageEntity(entityManager, "loading2", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg2->getMngr()->setActive(_loadimg2, false);

	_loadimg3 = entityFactory->CreateImageEntity(entityManager, "loading3", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg3->getMngr()->setActive(_loadimg3, false);

	_loadimg4 = entityFactory->CreateImageEntity(entityManager, "loading4", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg4->getMngr()->setActive(_loadimg4, false);

	_setRoomAudio();
	_setGlobalFeatures();
	_setRoomBackground();
	_setInteractuables();
	_setUI();
	_setDialog();
	_setCaseResolution();
	roomEvent[InitialDialogue]();
	SDL_Delay(1000);
	

}
void Room2Scene::resolvedPuzzle(int i)
{
	if (i < 6) {
		int auxevent = event_size;
		if (i == 0)  auxevent = TombPuzzleSceneRsv;
		else if (i == 1)  auxevent = RavenSceneRsv;
		else if (i == 2)  auxevent = DoorSceneRsv;
		else if (i == 3)  auxevent = MosaicPuzzleSceneRsv;
		else if (i == 4)  auxevent = OrganPuzzleSceneRsv;
		else if (i == 5)  auxevent = WindowSceneResolved;
		roomEvent[auxevent]();
	}
}

void Room2Scene::unload()
{
	entityManager->~EntityManager();
}

void Room2Scene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);

	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);
}

void Room2Scene::_setRoomEvents()
{
	roomEvent.resize(event_size);
#pragma region Events
	roomEvent[InitialDialogue] = [this] {
		if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("Sala2Intro_P2");
		else  startDialogue("Sala2Intro_P1");
		};
	roomEvent[CorpseDialogue] = [this] {
		entityManager->setActive(rmObjects.zoomCorpse, true);
		entityManager->setActive(rmObjects.quitButton, true);
		if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("CADAVER_P2");
		else {
			startDialogue("CADAVER_P1");
		}
		};
	roomEvent[TombPuzzleScene] = [this] {
		Game::Instance()->getSceneManager()->loadScene(TOMB_SCENE, this); 
		};
	roomEvent[TombPuzzleSceneRsv] = [this] {
		
		};
	roomEvent[RavenScene] = [this] {
		HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
		rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
		Game::Instance()->getSceneManager()->loadScene(RAVEN_SCENE, this);
		};
	roomEvent[RavenSceneRsv] = [this] {

		};
	roomEvent[DoorScene] = [this] {
		HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
		rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
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
		brokenMosaic = true;
		entityManager->getComponent<Image>(rmObjects.mosaic)->setTexture(&sdlutils().images().at("MosaicoRotoSala"));
		if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA)) startDialogue("MOSAICO3_2P");
		else {
			startDialogue("MOSAICO3_1P");
		}
		};
	roomEvent[OrganZoom] = [this] {
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
		
		startDialogue("Prueba");
		rmObjects.zoomOrgan->getMngr()->setActive(rmObjects.zoomOrgan, true);
		rmObjects.quitButton->getMngr()->setActive(rmObjects.quitButton, true);
		rmObjects.organ->getMngr()->setActive(rmObjects.organ, true);
		rmObjects.window->getMngr()->setActive(rmObjects.window, false);
		if (rmObjects.rope != nullptr) rmObjects.rope->getMngr()->setActive(rmObjects.rope, true);
		};
	roomEvent[Rope] = [this] {
		//Remove rope
		rmObjects.rope->getMngr()->removeEntity(rmObjects.rope);
		rmObjects.rope->getMngr()->setActive(rmObjects.rope, false);
		// InventoryLogic
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
		if (variant != 0) {
			GetInventory()->addItem(new Hint("CuerdaGruesa", sdlutils().invDescriptions().at("CuerdaGruesa"), &sdlutils().images().at("CuerdaGruesa")));
			GetInventory()->hints.push_back(entityFactory->CreateInvEntity(entityManager, "CuerdaGruesa", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
			GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		}
		else {
			GetInventory()->addItem(new Hint("CuerdaFina", sdlutils().invDescriptions().at("CuerdaFina"), &sdlutils().images().at("CuerdaFina")));
			GetInventory()->hints.push_back(entityFactory->CreateInvEntity(entityManager, "CuerdaFina", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
			GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		}
		createDescription(inv->hints.back(), inv->getItems().back());
		};
	roomEvent[WindowScene] = [this] {
		ckeckWindow = true;
		HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
		rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
		Game::Instance()->getSceneManager()->loadScene(WINDOW_SCENE, this);
		};
	roomEvent[WindowSceneResolved] = [this] {
		if(finishallpuzzles)roomEvent[ResolveCase]();
		};
	roomEvent[OrganPuzzleScene] = [this] {
		HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
		rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
		Game::Instance()->getSceneManager()->loadScene(MUSIC_PUZZLE, this);
		};
	roomEvent[OrganPuzzleSceneRsv] = [this] {
		brokenMosaic = false;
		MirrorMosaic = true;
		auto ZoomAccess = entityFactory->CreateInteractableEntity(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(720, 0), Vector2D(0, 0), 627 / 3, 1377 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(ZoomAccess));
		entityManager->getComponent<ClickComponent>(ZoomAccess)->connect(ClickComponent::JUST_CLICKED, [this]() {
			roomEvent[OrganZoom](); 
			});
		entityManager->getComponent<Image>(rmObjects.mosaic)->setTexture(&sdlutils().images().at("MosaicoEspejoSala"));
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
		if (variant == 2 && ckeckWindow) {
			roomEvent[ResolveCase]();
		}
		finishallpuzzles = true;
		};
	roomEvent[SecretEntry] = [this] {
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
		if (variant == 2) {
			if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("SalidaSecreta1_2PB");
			else startDialogue("SalidaSecreta1_1PB");
		}
		else {
			if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA))startDialogue("SalidaSecreta1_2PA");
			else startDialogue("SalidaSecreta1_1PA");
		}
		rmObjects.secretEntryZoom->getMngr()->setActive(rmObjects.secretEntryZoom, true);
		rmObjects.quitButton->getMngr()->setActive(rmObjects.quitButton, true);
		};
	roomEvent[ResolveCase] = [this] {
		//IMPORTANT assign dialogue
		startDialogue("Sala2Final");
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
		entityManager->setActive(rmObjects.readyToResolveBotton, true);
		};
	roomEvent[GoodEnd] = [this] {
		// WIP
		Game::Instance()->getDataManager()->SetSceneCount(SceneCount::MIDDLEROOM3);
		if(Game::Instance()->getDataManager()->GetCharacterState(KEISARA))_loadimg1->getMngr()->setActive(_loadimg1, true);
		else _loadimg2->getMngr()->setActive(_loadimg2, true);
		entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
		std::shared_ptr<Sound> correctSound = sdlutils().soundEffects().at("correcto");
		AudioManager::Instance().playSound(correctSound);
		Game::Instance()->render();
		Game::Instance()->getSceneManager()->popScene();
		};
	roomEvent[BadEnd] = [this] {
		// WIP
		Game::Instance()->getDataManager()->SetCharacterDead(Character::SOL);
		Game::Instance()->getDataManager()->SetSceneCount(SceneCount::MIDDLEROOM3);
		if (!Game::Instance()->getDataManager()->GetCharacterState(KEISARA))_loadimg3->getMngr()->setActive(_loadimg3, true);
		else _loadimg4->getMngr()->setActive(_loadimg4, true);
		entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
		std::shared_ptr<Sound> incorrectSound = sdlutils().soundEffects().at("incorrecto");
		AudioManager::Instance().playSound(incorrectSound);
		Game::Instance()->render();
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

#pragma region InitScroll

	auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "PuertaMausoleo", EntityFactory::RECTAREA, Vector2D(30 + 1359, 175), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12.0) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "PuertaCementerio", EntityFactory::RECTAREA, Vector2D(1218, 140), Vector2D(0, 0), 117, 428, 0, areaLayerManager, 12, ((sdlutils().width()) / 12.0) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

	auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);

	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom1));
	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));

	ChangeRoomScroll->setEndScrollCallback([this]() {scrolling = false; });

	rmObjects.backgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);

#pragma endregion

#pragma region Background

	//LeftBackground
	auto CementeryBackground = entityFactory->CreateImageEntity(entityManager, "FondoCementerio", Vector2D(0, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(CementeryBackground));

	//RightBackground
	auto MausoleumBackground = entityFactory->CreateImageEntity(entityManager, "MausoleoSala2", Vector2D(1359, 0), Vector2D(0, 0), 1359, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(MausoleumBackground));


#pragma endregion

#pragma region Scroll

	entityManager->getComponent<ClickComponent>(ChangeRoom2)->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom2]() {
		if (!isOpen && !rmObjects.backgroundScroll->isScrolling()) {
			if(rmObjects.backgroundScroll->Scroll(ScrollComponent::LEFT)) {
				auto trChangeRoom2 = entityManager->getComponent<Transform>(ChangeRoom2);
				trChangeRoom2->setPos(Vector2D(1218, 140));
				AudioManager::Instance().playSound(sdlutils().soundEffects().at("AbrirPuertaPiedra"));
				scrolling = true;
			}
		}
		else if (isOpen) {
			roomEvent[DoorScene]();
		}
		});

	entityManager->getComponent<ClickComponent>(ChangeRoom1)->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoomScroll, ChangeRoom1]() {
		if (!isOpen && !rmObjects.backgroundScroll->isScrolling()) {
			if (rmObjects.backgroundScroll->Scroll(ScrollComponent::RIGHT)) {
				auto trChangeRoom1 = entityManager->getComponent<Transform>(ChangeRoom1);
				trChangeRoom1->setPos(Vector2D(30, 175));
				AudioManager::Instance().playSound(sdlutils().soundEffects().at("AbrirPuertaPiedra"));
				scrolling = true;
			}
		}
		});

#pragma endregion

}

void Room2Scene::_setCaseResolution()
{
	//set the scene the variant is 
	Game::Instance()->getDataManager()->SetSceneCount(ROOM2);

	//get actual variant
	int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(1);


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
				if (variantAct == 0) //if its the not correct variant one dies
				{
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
				if (variantAct != 0) //if its the not correct variant one dies
				{

					Game::Instance()->getDataManager()->SetCharacterDead(SOL);
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



#pragma region CementeryEntities


	rmObjects.zoomCorpse = entityFactory->CreateImageEntity(entityManager, "patrisio", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	RectArea2D* corpseZoomArea = entityManager->addComponent<RectArea2D>(rmObjects.zoomCorpse, areaLayerManager);
	entityManager->setActive(rmObjects.zoomCorpse, false);

	characterCorpse = entityFactory->CreateInteractableEntity(entityManager, "CadaverSol", EntityFactory::RECTAREA, Vector2D(20, 320), Vector2D(0, 0), 772 / 3, 734 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(characterCorpse));

	entityManager->getComponent<ClickComponent>(characterCorpse)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				if (!finishallpuzzles)roomEvent[CorpseDialogue]();
			});

	auto tomb = entityFactory->CreateInteractableEntity(entityManager, "Tumba", EntityFactory::RECTAREA, Vector2D(525, 155), Vector2D(0, 0), 627 / 3, 1233 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(tomb));
	entityManager->getComponent<ClickComponent>(tomb)->connect(ClickComponent::JUST_CLICKED, [this]() {
		roomEvent[TombPuzzleScene]();
		});

	auto raven = entityFactory->CreateInteractableEntity(entityManager, "Cuervo", EntityFactory::RECTAREA, Vector2D(890, 165), Vector2D(0, 0), 185 / 3, 196 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(raven));
	entityManager->getComponent<ClickComponent>(raven)->connect(ClickComponent::JUST_CLICKED, [this]() {
		roomEvent[RavenScene]();
		});

#pragma endregion

#pragma region MausoleumEntities

#pragma region PreSolveMosaic

	//Before Completing mosaic
	rmObjects.mosaic = entityFactory->CreateInteractableEntity(entityManager, "MosaicoSala", EntityFactory::RECTAREA, Vector2D(370 + 1344, 0), Vector2D(0, 0), 1881 / 3, 1377 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.mosaic));
	entityManager->getComponent<ClickComponent>(rmObjects.mosaic)->connect(ClickComponent::JUST_CLICKED, [this]() {
		if (brokenMosaic) {
			roomEvent[OrganZoom]();
		}
		else if (MirrorMosaic) {
			roomEvent[SecretEntry]();
		}
		else roomEvent[MosaicPuzzleScene]();
		});

	rmObjects.window = entityFactory->CreateInteractableEntity(entityManager, "VentanaSala", EntityFactory::RECTAREA, Vector2D(1220 + 1344, 50), Vector2D(0, 0), 454 / 3, 792 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.window));
	entityManager->getComponent<ClickComponent>(rmObjects.window)->connect(ClickComponent::JUST_CLICKED, [this]() {
		roomEvent[WindowScene]();
		});

#pragma endregion

#pragma region SolvedMosaic

	//Zoomed Items

	rmObjects.zoomOrgan = entityFactory->CreateImageEntity(entityManager, "FondoOrgano", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	rmObjects.zoomOrgan->getMngr()->setActive(rmObjects.zoomOrgan, false);

	rmObjects.organ = entityFactory->CreateInteractableEntity(entityManager, "Organo", EntityFactory::RECTAREA, Vector2D(410, 60), Vector2D(0, 0), 456, 591, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	entityManager->getComponent<ClickComponent>(rmObjects.organ)->connect(ClickComponent::JUST_CLICKED, [this]() {
		roomEvent[OrganPuzzleScene]();
		});
	rmObjects.organ->getMngr()->setActive(rmObjects.organ, false);

	int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
	if (variant != 0) {
		rmObjects.rope = entityFactory->CreateInteractableEntity(entityManager, "CuerdaGruesa", EntityFactory::RECTAREA, Vector2D(250, 500), Vector2D(0, 0), 150, 150, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
		
	}
	else {
		rmObjects.rope = entityFactory->CreateInteractableEntity(entityManager, "CuerdaFina", EntityFactory::RECTAREA, Vector2D(250, 500), Vector2D(0, 0), 150, 150, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	}
	entityManager->getComponent<ClickComponent>(rmObjects.rope)->connect(ClickComponent::JUST_CLICKED, [this]() {
		roomEvent[Rope]();
		});
	rmObjects.rope->getMngr()->setActive(rmObjects.rope, false);

#pragma endregion

#pragma region SolvedOrgan

	//After completeing organ
	rmObjects.secretEntryZoom = entityFactory->CreateInteractableEntityNotMoveSprite(entityManager, "SalidaSecretaCerrada",EntityFactory::RECTAREA, Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0,areaLayerManager,EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	rmObjects.secretEntryZoom->getMngr()->getComponent<ClickComponent>(rmObjects.secretEntryZoom)->connect(ClickComponent::JUST_CLICKED, [this]() {
		int variant = Game::Instance()->getDataManager()->GetRoomVariant(1);
		if (variant == 2)
		{
			rmObjects.secretEntryZoom->getMngr()->getComponent<Image>(rmObjects.secretEntryZoom)->setTexture(&sdlutils().images().at("SalidaSecretaAbierta"));
		}
	});
	rmObjects.secretEntryZoom->getMngr()->setActive(rmObjects.secretEntryZoom, false);

#pragma endregion

#pragma endregion

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
	rmObjects.quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);

	entityManager->getComponent<ClickComponent>(rmObjects.quitButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				auto ImagequitButton = entityManager->getComponent<Image>(rmObjects.quitButton);
				ImagequitButton->setW(entityManager->getComponent<Transform>(rmObjects.quitButton)->getWidth());
				ImagequitButton->setH(entityManager->getComponent<Transform>(rmObjects.quitButton)->getHeight());
				ImagequitButton->setPosOffset(0, 0);
				AudioManager::Instance().playSound(rmSounds.uiButton);
				entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
				entityManager->setActive(rmObjects.quitButton, false);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);

			});

	entityManager->setActive(rmObjects.quitButton, false);
#pragma endregion

#pragma region Inventory

	//description text entity
	invObjects.textDescriptionEnt = entityManager->addEntity(ecs::grp::UI);
	auto _testTextTranform = entityManager->addComponent<Transform>(invObjects.textDescriptionEnt, Vector2D(600, 300), Vector2D(0, 0), 300, 200, 0);
	entityManager->setActive(invObjects.textDescriptionEnt, false);
	SDL_Color colorDialog = { 255, 255, 255, 255 };
	entityManager->addComponent<WriteTextComponent<DescriptionInfo>>(invObjects.textDescriptionEnt, sdlutils().fonts().at("BASE"), colorDialog, GetInventory()->getTextDescription());


	//Inventory

	invObjects.InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
	entityManager->addComponent<InvAnimComponent>(invObjects.InventoryBackground);
	rmObjects.inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.InventoryBackground, false);

	invObjects.InvArea = entityManager->addComponent<RectArea2D>(invObjects.InventoryBackground, areaLayerManager);

	invObjects.inventoryUpButton = entityFactory->CreateInvEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.inventoryUpButton, false);

	invObjects.inventoryDownButton = entityFactory->CreateInvEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(invObjects.inventoryDownButton, false);

	entityManager->getComponent<ClickComponent>(rmObjects.inventoryButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.uiButton);
				GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

				if (GetInventory()->getActive()) // If the inventory is active, activate the items
				{
					entityManager->setActive(invObjects.InventoryBackground, true);
					entityManager->getComponent<InvAnimComponent>(invObjects.InventoryBackground)->startInvAnim();
					entityManager->setActive(rmObjects.logbtn, false);
					//change the position of the log button
					areaLayerManager->sendFront(invObjects.InvArea->getLayerPos());
					

					areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryUpButton)->getLayerPos());
					areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(invObjects.inventoryDownButton)->getLayerPos());

					entityManager->setActive(invObjects.inventoryDownButton, true);
					entityManager->setActive(invObjects.inventoryUpButton, true);

					entityManager->getComponent<InvAnimComponent>(invObjects.inventoryDownButton)->startInvAnim();
					entityManager->getComponent<InvAnimComponent>(invObjects.inventoryUpButton)->startInvAnim();

					for (int i = inv->getFirstItem(); i < inv->getItemNumber() + inv->getFirstItem(); ++i) {
						inv->hints[i]->getMngr()->setActive(inv->hints[i], true);  // Activate the items
						areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inv->hints[i])->getLayerPos());
						inv->hints[i]->getMngr()->getComponent<InvAnimComponent>(inv->hints[i])->startInvAnim();
					}
				}
				else
				{

					entityManager->getComponent<InvAnimComponent>(invObjects.inventoryDownButton)->endInvAnim();
					entityManager->getComponent<InvAnimComponent>(invObjects.inventoryUpButton)->endInvAnim();
					entityManager->getComponent<InvAnimComponent>(invObjects.InventoryBackground)->endInvAnim();
					entityManager->setActive(rmObjects.logbtn, true);
					for (int i = inv->getFirstItem(); i < inv->getItemNumber() + inv->getFirstItem(); ++i) inv->hints[i]->getMngr()->getComponent<InvAnimComponent>(inv->hints[i])->endInvAnim();// Desactivate the items 

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

	entityManager->setActive(rmObjects.quitButton, false);
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
