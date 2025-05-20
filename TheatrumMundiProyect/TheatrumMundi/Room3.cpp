#include "Room3.h"
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
#include "BehaviorStateComponent.h"
#include "InvAnimComponent.h"

Room3Scene::Room3Scene()
{
	//Creation of the DialogueManager of the room and creation of the events 
	dialogueManager = new DialogueManager(1);
	_setRoomEvents();
}

Room3Scene::~Room3Scene()
{
}

void Room3Scene::init()
{
	if (isStarted) return;
	isStarted = true;
	//isOpen = false;
	finishallpuzzles = false;
	_setRoomAudio();
	_setGlobalFeatures();
	_setRoomBackground();
	_setInteractuables();
	_setUI();
	_setDialog();
	_setCaseResolution();
	//roomEvent[InitialDialogue]();

	//Load Images
	_setLoadImages();


	SDL_Delay(1000);
}

void Room3Scene::resolvedPuzzle(int i)
{
	if (i < 5) {
		int auxevent = event_size;
		if (i == 0)  auxevent = BalanceSceneRsv;
		else if (i == 1)  auxevent = WiresSceneRsv;
		else if (i == 2)  auxevent = CircleLockSceneRsv;
		else if (i == 3)  auxevent = ParrotSceneRsv;
		//else if (i == 4)  auxevent = OrganPuzzleSceneRsv;
		roomEvent[auxevent]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		if (aux) entityManager->setActive(characterCorpse, true);
	}
}

void Room3Scene::unload()
{
	entityManager->~EntityManager();
}

void Room3Scene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);

	
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);

	if (finishallpuzzles)
	{
		roomEvent[ResolveButtons]();
	}
}

void Room3Scene::_setRoomEvents()
{
	roomEvent.resize(event_size);

#pragma region Events
	roomEvent[InitialDialogue] = [this] {
		//startDialogue("SalaIntermedia3");
		};
	roomEvent[CorpseDialogue] = [this] {
		entityManager->setActive(rmObjects.zoomCorpse, true);
		entityManager->setActive(rmObjects.quitButton, true);
		//startDialogue("Cadaver");
		};
	roomEvent[LightsOn] = [this] {
		Game::Instance()->getDataManager()->SetRoom3Phase(Phase::LIGHTS_ON);
		//LightsToOn = true; //To interact with determined entities
		//LightsToRed = false;
		entityManager->getComponent<Image>(rmObjects.backgroundLeftIllumination)->setTexture(&sdlutils().images().at("FondoSalaDeEspera"));
		entityManager->getComponent<Image>(rmObjects.backgroundRightIllumination)->setTexture(&sdlutils().images().at("FondoJuzgado"));
		entityManager->getComponent<Image>(rmObjects.boxOfficeMorseCodeB)->setTexture(&sdlutils().images().at("TaquillaAzul"));
		entityManager->getComponent<Image>(rmObjects.boxOfficeCircleLockP)->setTexture(&sdlutils().images().at("TaquillaMorada"));
		entityManager->getComponent<Image>(rmObjects.cablesPuzzle)->setTexture(&sdlutils().images().at("Cables"));
		entityManager->getComponent<Image>(rmObjects.parrot)->setTexture(&sdlutils().images().at("Parrot"));
		entityManager->getComponent<Image>(rmObjects.zoomCorpse)->setTexture(&sdlutils().images().at("Cadaver3"));
		entityManager->getComponent<Image>(rmObjects.balance)->setTexture(&sdlutils().images().at("Balanza"));
		entityManager->getComponent<Image>(rmObjects.locker)->setTexture(&sdlutils().images().at("CajaFuerte"));
		entityManager->getComponent<Image>(rmObjects.radio)->setTexture(&sdlutils().images().at("Radio"));
		//characterCorpse
		entityManager->getComponent<Image>(rmObjects.zoomCorpse)->setTexture(&sdlutils().images().at("Cadaver3"));
		if (characterCorpse != nullptr) entityManager->getComponent<Image>(characterCorpse)->setTexture(&sdlutils().images().at("Cadaver3"));

		entityManager->getComponent<BehaviorStateComponent>(rmObjects.parrot)->setState(Phase::LIGHTS_ON);
		};
	roomEvent[LightsOff] = [this] {
		//LightsToOn = false; //To not interact with determined entities
		//LightsToRed = false;
		Game::Instance()->getDataManager()->SetRoom3Phase(Phase::LIGHTS_OFF);
		//entityManager->getComponent<Image>(rmObjects.backgroundLeftIllumination)->setTexture(&sdlutils().images().at("FondoSalaDeEsperaOscuro"));
		//entityManager->getComponent<Image>(rmObjects.backgroundRightIllumination)->setTexture(&sdlutils().images().at("FondoJuzgadoOscuro"));
		//entityManager->getComponent<Image>(rmObjects.boxOfficeMorseCodeB)->setTexture(&sdlutils().images().at("TaquillaAzulOscuro"));
		//entityManager->getComponent<Image>(rmObjects.boxOfficeCircleLockP)->setTexture(&sdlutils().images().at("TaquillaMoradaOscuro"));
		//entityManager->getComponent<Image>(rmObjects.cablesPuzzle)->setTexture(&sdlutils().images().at("CablesOscuro"));
		//entityManager->getComponent<Image>(rmObjects.parrot)->setTexture(&sdlutils().images().at("ParrotOscuro"));
		//entityManager->getComponent<Image>(rmObjects.balance)->setTexture(&sdlutils().images().at("BalanzaOscuro"));
		//entityManager->getComponent<Image>(rmObjects.locker)->setTexture(&sdlutils().images().at("CajaFuerteOscuro"));

		entityManager->getComponent<Image>(rmObjects.backgroundLeftIllumination)->setTexture(&sdlutils().images().at("FondoSalaDeEsperaOscuro"));
		entityManager->getComponent<Image>(rmObjects.backgroundRightIllumination)->setTexture(&sdlutils().images().at("FondoJuzgadoOscuro"));
		entityManager->getComponent<Image>(rmObjects.boxOfficeMorseCodeB)->setTexture(&sdlutils().images().at("EmptyImage"));
		entityManager->getComponent<Image>(rmObjects.boxOfficeCircleLockP)->setTexture(&sdlutils().images().at("EmptyImage"));
		entityManager->getComponent<Image>(rmObjects.cablesPuzzle)->setTexture(&sdlutils().images().at("CablesOscuro"));
		entityManager->getComponent<Image>(rmObjects.parrot)->setTexture(&sdlutils().images().at("EmptyImage"));
		entityManager->getComponent<Image>(rmObjects.balance)->setTexture(&sdlutils().images().at("EmptyImage"));
		entityManager->getComponent<Image>(rmObjects.locker)->setTexture(&sdlutils().images().at("EmptyImage"));
		entityManager->getComponent<Image>(rmObjects.radio)->setTexture(&sdlutils().images().at("EmptyImage"));
		//characterCorpse
		entityManager->getComponent<Image>(rmObjects.zoomCorpse)->setTexture(&sdlutils().images().at("Cadaver3Oscuro"));
		if (characterCorpse != nullptr) entityManager->getComponent<Image>(characterCorpse)->setTexture(&sdlutils().images().at("EmptyImage"));

		entityManager->getComponent<BehaviorStateComponent>(rmObjects.parrot)->setState(Phase::LIGHTS_OFF);
		};
	roomEvent[LightsRed] = [this] {
		//LightsToOn = true;  //To interact with determined entities when the room is red
		//LightsToRed = true;
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		Game::Instance()->getDataManager()->SetRoom3Phase(Phase::LIGHTS_RED);
		entityManager->getComponent<Image>(rmObjects.backgroundLeftIllumination)->setTexture(&sdlutils().images().at("FondoSalaDeEsperaRojo"));
		entityManager->getComponent<Image>(rmObjects.backgroundRightIllumination)->setTexture(&sdlutils().images().at("FondoJuzgadoRojo"));
		entityManager->getComponent<Image>(rmObjects.boxOfficeMorseCodeB)->setTexture(&sdlutils().images().at("TaquillaAzulRojo"));
		entityManager->getComponent<Image>(rmObjects.boxOfficeCircleLockP)->setTexture(&sdlutils().images().at("TaquillaMoradaRojo"));
		entityManager->getComponent<Image>(rmObjects.cablesPuzzle)->setTexture(&sdlutils().images().at("CablesRojo"));
		entityManager->getComponent<Image>(rmObjects.parrot)->setTexture(&sdlutils().images().at("ParrotRojo"));
		entityManager->getComponent<Image>(rmObjects.balance)->setTexture(&sdlutils().images().at("BalanzaRojo"));
		entityManager->getComponent<Image>(rmObjects.locker)->setTexture(&sdlutils().images().at("CajaFuerteRojo"));
		entityManager->getComponent<Image>(rmObjects.radio)->setTexture(&sdlutils().images().at("Radio"));
		//characterCorpse
		entityManager->getComponent<Image>(rmObjects.zoomCorpse)->setTexture(&sdlutils().images().at("Cadaver3Rojo"));
		if (characterCorpse != nullptr) entityManager->getComponent<Image>(characterCorpse)->setTexture(&sdlutils().images().at("Cadaver3Rojo"));

		entityManager->getComponent<BehaviorStateComponent>(rmObjects.parrot)->setState(Phase::LIGHTS_RED);
		};
	roomEvent[BalancePuzzleScene] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		_resetSounds();
		Game::Instance()->getSceneManager()->loadScene(BALANCE_PUZZLE, this);
		};
	roomEvent[WiresPuzzleScene] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		_resetSounds();
		Game::Instance()->getSceneManager()->loadScene(WIRES_PUZZLE, this);
		};
	roomEvent[WiresSceneRsv] = [this] {
			roomEvent[LightsOn]();
		};
	roomEvent[CircleLockPuzzleScene] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		_resetSounds();
		Game::Instance()->getSceneManager()->loadScene(LOCKER_PUZZLE, this);
		};
	roomEvent[CircleLockSceneRsv] = [this] {

		};
	roomEvent[MorseCodePuzzleScene] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		_resetSounds();
		Game::Instance()->getSceneManager()->loadScene(BOX, this);
		};
	roomEvent[MorseCodeSceneRsv] = [this] {

		};
	roomEvent[BalancePuzzleScene] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		_resetSounds();
		Game::Instance()->getSceneManager()->loadScene(BALANCE_PUZZLE, this);
		};
	roomEvent[BalanceSceneRsv] = [this] {
			roomEvent[LightsRed]();
		};
	roomEvent[ParrotScene] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		_resetSounds();
		Game::Instance()->getSceneManager()->loadScene(PARROT_PUZZLE, this);
		};
	roomEvent[ParrotSceneRsv] = [this] {
			roomEvent[ResolveCase]();
		};
	roomEvent[ZoomMorseGuide] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		entityManager->setActive(rmObjects.zoomMorseGuide, true);
		entityManager->setActive(rmObjects.quitButton, true);
		//feather
		if (rmObjects.feather != nullptr) { 
			entityManager->setActive(rmObjects.feather, true);
			
		}

		_resetSounds();

		};
	roomEvent[ZoomRadio] = [this] {
		parrotUtils.lastSoundTime = 0; //The timer from parrot and radio are together 
		parrotUtils.zoomParrotRadio = true;
		_resetSounds();
		entityManager->setActive(rmObjects.zoomRadio, true);
		entityManager->setActive(rmObjects.quitButton, true);
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
		entityManager->setActive(rmObjects.readyToResolveBotton, true);
		};
	roomEvent[GoodEnd] = [this] {
		// WIP
		Game::Instance()->getDataManager()->SetSceneCount(SceneCount::END);

		if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA) && Game::Instance()->getDataManager()->GetCharacterState(SOL)) {
			_loadimg1->getMngr()->setActive(_loadimg1, true);
		}
		else if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA) && !Game::Instance()->getDataManager()->GetCharacterState(SOL)) {
			_loadimg4->getMngr()->setActive(_loadimg4, true);
		}
		else if (!Game::Instance()->getDataManager()->GetCharacterState(KEISARA) && Game::Instance()->getDataManager()->GetCharacterState(SOL)) {
			_loadimg2->getMngr()->setActive(_loadimg2, true);
		}
		else _loadimg3->getMngr()->setActive(_loadimg3, true);
		
		entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
		std::shared_ptr<Sound> correctSound = sdlutils().soundEffects().at("correcto");
		AudioManager::Instance().playSound(correctSound);
		Game::Instance()->render();
		Game::Instance()->getSceneManager()->popScene();
		};
	roomEvent[BadEnd] = [this] {
		// WIP
		Game::Instance()->getDataManager()->SetCharacterDead(Character::LUCY);
		Game::Instance()->getDataManager()->SetSceneCount(SceneCount::END);

		if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA) && Game::Instance()->getDataManager()->GetCharacterState(SOL)) {
			_loadimg6->getMngr()->setActive(_loadimg6, true);
		}
		else if (Game::Instance()->getDataManager()->GetCharacterState(KEISARA) && !Game::Instance()->getDataManager()->GetCharacterState(SOL)) {
			_loadimg5->getMngr()->setActive(_loadimg5, true);
		}
		else if (!Game::Instance()->getDataManager()->GetCharacterState(KEISARA) && Game::Instance()->getDataManager()->GetCharacterState(SOL)) {
			_loadimg7->getMngr()->setActive(_loadimg7, true);
		}
		else _loadimg8->getMngr()->setActive(_loadimg8, true);
		
		entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
		std::shared_ptr<Sound> incorrectSound = sdlutils().soundEffects().at("incorrecto");
		AudioManager::Instance().playSound(incorrectSound);
		Game::Instance()->render();
		Game::Instance()->getSceneManager()->popScene();
		};
#pragma endregion
}

void Room3Scene::_setRoomAudio()
{
	//Audio sfx 
	AudioManager& audioMngr = AudioManager::Instance();

	rmSounds.uiButton = sdlutils().soundEffects().at("boton");
	audioMngr.setVolume(rmSounds.uiButton, 0.2);

	rmSounds.puzzleButton = sdlutils().soundEffects().at("puzzle");
	audioMngr.setVolume(rmSounds.puzzleButton, 0.3);

	rmSounds.doorSound = sdlutils().soundEffects().at("puerta");

	rmSounds.explosionSound = sdlutils().soundEffects().at("explosion");

	rmSounds.s_Sound = sdlutils().soundEffects().at("sSoundLow");
	rmSounds.t_Sound = sdlutils().soundEffects().at("tSoundLow");
	rmSounds.o_Sound = sdlutils().soundEffects().at("oSoundLow");
	rmSounds.p_Sound = sdlutils().soundEffects().at("pSoundLow");

	rmSounds.morse_Sound = sdlutils().soundEffects().at("MorseSound");
	rmSounds.morse_Sound_Low = sdlutils().soundEffects().at("MorseSoundLow");
	rmSounds.shootSound = sdlutils().soundEffects().at("shootShound");

	//rmSounds.stopSound = sdlutils().soundEffects().at("");
	/*Audio music
	Sound room1music = sdlutils().musics().at("sala2");
	audioMngr.setLooping(room1music, true);
	audioMngr.playSound(room1music);*/
}

void Room3Scene::_setGlobalFeatures()
{
	//Register scene in dialogue manager
	dialogueManager->setScene(this);
}

void Room3Scene::_setRoomBackground()
{
#pragma region InitScroll

	auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1160 - 1349, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

	auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom1));
	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));
	ChangeRoomScroll->setEndScrollCallback([this]() {scrolling = false; });

	rmObjects.backgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);

#pragma endregion

#pragma region Background

	//LeftBackground
	rmObjects.backgroundLeftIllumination = entityFactory->CreateImageEntity(entityManager, "FondoSalaDeEsperaOscuro", Vector2D(-1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.backgroundLeftIllumination));
	
	//RightBackground
	rmObjects.backgroundRightIllumination = entityFactory->CreateImageEntity(entityManager, "FondoJuzgadoOscuro", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.backgroundRightIllumination));

#pragma endregion

#pragma region Scroll

	entityManager->getComponent<ClickComponent>(ChangeRoom1)
		->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoomScroll, ChangeRoom2]()
			{
				if (!rmObjects.backgroundScroll->isScrolling()) {
					if (rmObjects.backgroundScroll->Scroll(ScrollComponent::RIGHT)) {
					auto trChangeRoom2 = entityManager->getComponent<Transform>(ChangeRoom2);
					trChangeRoom2->setPos(Vector2D(1160 - 1349, 160));
					AudioManager::Instance().playSound(rmSounds.doorSound); //If you can scroll, scroll and play the door sound
					scrolling = true;
					}
				}
			});

	entityManager->getComponent<ClickComponent>(ChangeRoom2)
		->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom1]()
			{
				if (!rmObjects.backgroundScroll->isScrolling()) {
					if (rmObjects.backgroundScroll->Scroll(ScrollComponent::LEFT)) {
					auto trChangeRoom1 = entityManager->getComponent<Transform>(ChangeRoom1);
					trChangeRoom1->setPos(Vector2D(34 + 1349, 160));
					AudioManager::Instance().playSound(rmSounds.doorSound); //If you can scroll, scroll and play the door sound
					scrolling = true;
					}
				}
			});
#pragma endregion
}

void Room3Scene::_setCaseResolution()
{
	//set the scene the variant is 
	Game::Instance()->getDataManager()->SetSceneCount(ROOM1);

	//get actual variant
	int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(0);


	auto background = entityFactory->CreateImageEntity(
		entityManager, "fondoPruebaLog", Vector2D(2, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DECISION);

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

					Game::Instance()->getDataManager()->SetCharacterDead(LUCY);
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

void Room3Scene::_setInteractuables()
{
	//CORPSE
	rmObjects.zoomCorpse = entityFactory->CreateImageEntity(entityManager, "Cadaver3Oscuro", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	//RectArea2D* corpseZoomArea = entityManager->addComponent<RectArea2D>(rmObjects.zoomCorpse, areaLayerManager);
	entityManager->setActive(rmObjects.zoomCorpse, false);

	characterCorpse = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(1000, 300), Vector2D(0, 0), 250 / 3, 225 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(characterCorpse));

	entityManager->getComponent<ClickComponent>(characterCorpse)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				if (!finishallpuzzles && Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
					roomEvent[CorpseDialogue]();
				}
			});

	//BOXOFFICE CIRCLE LOCK
	rmObjects.boxOfficeMorseCodeB = entityFactory->CreateInteractableEntity(entityManager,"EmptyImage",EntityFactory::RECTAREA,Vector2D(-1200,300),Vector2D(0,0),100,100,0,areaLayerManager,EntityFactory::NODRAG,ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.boxOfficeMorseCodeB)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[CircleLockPuzzleScene]();
		}

		});
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.boxOfficeMorseCodeB));

	//BOXOFFICE ZOOM MORSE GUIDE
	rmObjects.zoomMorseGuide = entityFactory->CreateImageEntity(entityManager, "taquillaAbierta", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	entityManager->setActive(rmObjects.zoomMorseGuide, false);

	//feather 
	rmObjects.feather = entityFactory->CreateInteractableEntity(entityManager, "pluma", EntityFactory::RECTAREA, Vector2D(1000, 200), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->getComponent<ClickComponent>(rmObjects.feather)->connect(ClickComponent::JUST_CLICKED, [this]() {
		GetInventory()->addItem(new Hint("pluma", sdlutils().invDescriptions().at("pluma"), &sdlutils().images().at("pluma")));
		inv->hints.push_back(entityFactory->CreateInvEntity(entityManager, "pluma", EntityFactory::RECTAREA, inv->setPosition(), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
		createDescription(inv->hints.back(), inv->getItems().back());
		if (inv->getActive()) inv->hints.back()->getMngr()->setActive(inv->hints.back(), true);
		else inv->hints.back()->getMngr()->setActive(inv->hints.back(), false);
		entityManager->setActive(rmObjects.feather, false);
		rmObjects.feather = nullptr;
		
		});
	entityManager->setActive(rmObjects.feather, false);
	rmObjects.boxOfficeCircleLockP = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(-1000,200), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.boxOfficeCircleLockP)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[ZoomMorseGuide]();
		}

		});
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.boxOfficeCircleLockP));

	//PARROT
	rmObjects.parrot = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(1000, 0), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.parrot)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[ParrotScene]();
		}

		});
	
	BehaviorStateComponent* parrotStateCom = entityManager->addComponent<BehaviorStateComponent>(rmObjects.parrot);

	parrotUtils.codeSequenceSounds.push_back(rmSounds.shootSound); // TODO: Gunshoot
	parrotUtils.codeSequenceSounds.push_back(rmSounds.s_Sound); // TODO: S
	parrotUtils.codeSequenceSounds.push_back(rmSounds.t_Sound); // TODO: T
	parrotUtils.codeSequenceSounds.push_back(rmSounds.o_Sound); // TODO: O
	parrotUtils.codeSequenceSounds.push_back(rmSounds.p_Sound); // TODO: P
	//parrotUtils.codeSequenceSounds.push_back(rmSounds.explosionSound); // TODO: Explosion

	//FASES DEL PARROT
	//Sin luz, sin ruido en ningun zoom ni de lejos ni de cerca
	//Luz azul, sonidos de disparos en volumen bajo de lejos y en volumen alto de cerca (gestion del ParrotPuzzleScene)
	//Luz roja, STOP, sin sonido de disparo, en volumen bajo de lejos y en volumen alto de cerca (gestion del ParrotPuzzleScene)
	//Linterna, explosion, solo si estamos en fase roja

	//BEHAVIOURS OF THE PARROT
	auto shootingBehavior = [parrotStateCom, this] // SHOOTING_SOUND
	() {
		if (rmObjects.backgroundScroll->startPhaseCheck()) { //Right side of the room PARROT
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 1000 && Game::Instance()->getDataManager()->GetRoom3Phase() > 0) { // Every second
				AudioManager::Instance().setVolume(rmSounds.shootSound, 0.1f);
				AudioManager::Instance().playSound(parrotUtils.codeSequenceSounds[0]);
				parrotUtils.lastSoundTime = sdlutils().currTime();
			}
		}
		else { //Left side of the room MORSE (if we have the LightsOnState)
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 8000 && Game::Instance()->getDataManager()->GetRoom3Phase() == 2) { // Every eight seconds and lights on
				if (parrotUtils.zoomParrotRadio) AudioManager::Instance().playSound(rmSounds.morse_Sound);
				else AudioManager::Instance().playSound(rmSounds.morse_Sound_Low);
				parrotUtils.lastSoundTime = sdlutils().currTime();
			}
		}

		parrotStateCom->setState(Game::Instance()->getDataManager()->GetRoom3Phase()); // Check if changes the room state
		};

	auto stopBehavior = [parrotStateCom, this]
	() {
		if (rmObjects.backgroundScroll->startPhaseCheck()) { //Right side of the room PARROT
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 1000 && Game::Instance()->getDataManager()->GetRoom3Phase() > 0) { // Every second
		

				if (parrotUtils.codeSeqIteration == parrotUtils.codeSequenceSounds.size() - 1) {
					parrotUtils.codeSeqIteration == 1;
				} //Not repeat the explosion

				if (parrotUtils.codeSeqIteration == 0) {
					++parrotUtils.codeSeqIteration;
				} //Not repeat the shoot

				AudioManager::Instance().playSound(parrotUtils.codeSequenceSounds[parrotUtils.codeSeqIteration]);
				
				++parrotUtils.codeSeqIteration;

				parrotUtils.codeSeqIteration = parrotUtils.codeSeqIteration % parrotUtils.codeSequenceSounds.size();

				parrotUtils.lastSoundTime = sdlutils().currTime();
			}
		}
		else { //Left side of the room MORSE (if we have the redLightsState)
			if (sdlutils().currTime() - parrotUtils.lastSoundTime >= 8000 && Game::Instance()->getDataManager()->GetRoom3Phase() == 2) { // Every eight seconds
				if (parrotUtils.zoomParrotRadio) AudioManager::Instance().playSound(rmSounds.morse_Sound);
				else AudioManager::Instance().playSound(rmSounds.morse_Sound_Low);
				parrotUtils.lastSoundTime = sdlutils().currTime();
			}
		}

		parrotStateCom->setState(Game::Instance()->getDataManager()->GetRoom3Phase()); // Check if changes the room state
		};

	//STATES OF THE PARROT
	parrotStateCom->defBehavior(Phase::LIGHTS_OFF, 
		[parrotStateCom, this]() {

		}); //Nothing happens if the lights are off

	parrotStateCom->defBehavior(Phase::LIGHTS_ON,  shootingBehavior); //ShootSounds
	
	parrotStateCom->defBehavior(Phase::LIGHTS_RED, stopBehavior); // PUZZLE CODE REVEAL PHASE

	parrotStateCom->setState(Phase::LIGHTS_OFF); // The other will be setted after finishin the puzzle

	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.parrot));

	//CABLES
	rmObjects.cablesPuzzle = entityFactory->CreateInteractableEntity(entityManager, "CablesOscuro", EntityFactory::RECTAREA, Vector2D(-600, 200), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.cablesPuzzle)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (!rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[WiresPuzzleScene]();
		}

		});
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.cablesPuzzle));

	//BALANCE
	rmObjects.balance = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(800, 200), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.balance)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[BalancePuzzleScene]();
		}
			
		});
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.balance));

	//LOCKER
	rmObjects.locker = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(1200, 300), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.locker)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[MorseCodePuzzleScene]();
		}

		});
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.locker));

	//RADIO
	rmObjects.zoomRadio = entityFactory->CreateImageEntity(entityManager, "Radio", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	entityManager->setActive(rmObjects.zoomRadio, false);

	rmObjects.radio = entityFactory->CreateInteractableEntity(entityManager, "EmptyImage", EntityFactory::RECTAREA, Vector2D(-300, 300), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::DEFAULT);
	entityManager->getComponent<ClickComponent>(rmObjects.radio)->connect(ClickComponent::JUST_CLICKED, [this]() {
		
		if (Game::Instance()->getDataManager()->GetRoom3Phase() > 0 && !rmObjects.backgroundScroll->isScrolling()) {
			roomEvent[ZoomRadio]();
		}

	});
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(rmObjects.radio));

}

void Room3Scene::_setDialog()
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

void Room3Scene::_setUI()
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

				parrotUtils.zoomParrotRadio = false;

				_resetSounds();

				parrotUtils.lastSoundTime = 0;

				if (rmObjects.feather != nullptr) {
					entityManager->setActive(rmObjects.feather, false);
				}


				//if the condition of this objects has not been apply disallow them
				//if (rmObjects.mirror.second == false) rmObjects.mirror.first->getMngr()->setActive(rmObjects.mirror.first, false);
				//if (rmObjects.organMosaic.second == false) rmObjects.organMosaic.first->getMngr()->setActive(rmObjects.organMosaic.first, false);
				//if (rmObjects.secretEntry.second == false) rmObjects.secretEntry.first->getMngr()->setActive(rmObjects.secretEntry.first, false);
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

void Room3Scene::_setLoadImages()
{
	_loadimg1 = entityFactory->CreateImageEntity(entityManager, "loading1", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg1->getMngr()->setActive(_loadimg1, false);

	_loadimg2 = entityFactory->CreateImageEntity(entityManager, "loading2", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg2->getMngr()->setActive(_loadimg2, false);

	_loadimg3 = entityFactory->CreateImageEntity(entityManager, "loading3", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg3->getMngr()->setActive(_loadimg3, false);

	_loadimg4 = entityFactory->CreateImageEntity(entityManager, "loading4", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg4->getMngr()->setActive(_loadimg4, false);

	_loadimg5 = entityFactory->CreateImageEntity(entityManager, "loading5", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg5->getMngr()->setActive(_loadimg5, false);

	_loadimg6 = entityFactory->CreateImageEntity(entityManager, "loading6", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg6->getMngr()->setActive(_loadimg6, false);

	_loadimg7 = entityFactory->CreateImageEntity(entityManager, "loading7", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg7->getMngr()->setActive(_loadimg7, false);

	_loadimg8 = entityFactory->CreateImageEntity(entityManager, "loading8", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg8->getMngr()->setActive(_loadimg8, false);
}

void Room3Scene::_resetSounds()
{
	AudioManager::Instance().stopSound(rmSounds.s_Sound);
	AudioManager::Instance().stopSound(rmSounds.t_Sound);
	AudioManager::Instance().stopSound(rmSounds.o_Sound);
	AudioManager::Instance().stopSound(rmSounds.p_Sound);
	AudioManager::Instance().stopSound(rmSounds.shootSound);
	AudioManager::Instance().stopSound(rmSounds.explosionSound);
	AudioManager::Instance().stopSound(rmSounds.morse_Sound);
	AudioManager::Instance().stopSound(rmSounds.morse_Sound_Low);
}
