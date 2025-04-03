  #include "Room1.h"
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
#include "../TheatrumMundi/EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"


#include "../src/components/WriteTextComponent.h"
#include "DialogueManager.h"

Room1Scene::Room1Scene() : SceneRoomTemplate(), _eventToRead("Sala1Intro")
{
	dialogueManager = new DialogueManager(1);
	
	_setRoomEvents();
}

Room1Scene::~Room1Scene()
{
}

void Room1Scene::init()
{
	if (isStarted) return;
	
	isStarted = true;
	finishallpuzzles = false;

	_setRoomAudio();

	_setGlobalFeatures();

	_setRoomBackground();

	_setCaseResolution();

	_setInteractuables();

	_setUI();

	_setDialog();

	roomEvent[InitialDialogue]();
	
	SDL_Delay(1000);
}



void Room1Scene::resolvedPuzzle(int i)
{
	if (i <4) {
		int auxevent = EVENTS_SIZE;
		if (i == 0)  auxevent = PipePuzzleRsv;
		else if (i == 1)  auxevent = BooksPuzzleRsv;
		else if (i == 2)  auxevent = ClockPuzzleRsv;
		else if (i == 3)  auxevent = TeaCupPuzzleRsv;
		roomEvent[auxevent]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		if (aux) entityManager->setActive(characterCorpse, true);
	}
	else {
#ifdef _DEBUG
		std::cout << i << " invalid index" << std::endl;
#endif
	}
}

void Room1Scene::refresh()
{
}

void Room1Scene::unload()
{
	entityManager->~EntityManager();
}

//// Private Internal Setting Methods

void Room1Scene::_setRoomEvents()
{
	roomEvent.resize(EVENTS_SIZE);

	roomEvent[InitialDialogue] = [this]()
		{ 
			startDialogue("Sala1Intro"); 
		};
	
	roomEvent[CorpseDialogue] = [this]()
		{
			roomEvent[ResolveCase]();

			_eventToRead = Cadaver;
			startDialogue("Cadaver");
		};

	roomEvent[PipePuzzleSnc] = [this]()
		{
			Game::Instance()->getSceneManager()->loadScene(PIPE_PUZZLE, this);
		};

	roomEvent[PipePuzzleRsv] = [this]() 
		{
			// InventoryLogic
		};

	roomEvent[BooksPuzzleScn] = [this]()
		{
			Game::Instance()->getSceneManager()->loadScene(BOOKS_PUZZLE, this);
		};

	roomEvent[BooksPuzzleRsv] = [this]()
		{
			// InventoryLogic
		};

	roomEvent[ClockPuzzleSnc] = [this]()
		{
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};

	roomEvent[ClockPuzzleRsv] = [this]()
		{
			// InventoryLogic
		};

	roomEvent[TeaCupPuzzleSnc] = [this]()
		{
			Game::Instance()->getSceneManager()->loadScene(TEA_CUP_PUZZLE, this);
		};

	roomEvent[TeaCupPuzzleRsv] = [this]()
		{
			// InventoryLogic
			entityManager->removeComponent<ClickComponent>(puzzleptr[4]);
		};
	roomEvent[Spoon] = [this]()
		{
			// InventoryLogic
			GetInventory()->addItem(new Hint("TeaCupSpoon", "Es una cuchara, que no lo ves o que", &sdlutils().images().at("TeaCupSpoon")));
			GetInventory()->hints.push_back(entityFactory->CreateInteractableEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, GetInventory()->setPosition(), Vector2D(0, 0), 268 / 2, 268 / 2, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
			GetInventory()->hints.back()->getMngr()->setActive(GetInventory()->hints.back(), false);
		};
			
	roomEvent[ResolveCase] = [this]()
		{
			startDialogue("Sala1Intro"); //Poner el dialogo correspondiente
		};

	roomEvent[GoodEnd] = [this]()
		{
			// WIP
			Game::Instance()->getSceneManager()->popScene();
		};

	roomEvent[BadEnd] = [this]()
		{
			// WIP
			Game::Instance()->getSceneManager()->popScene();
		};

	roomEvent[MobileDialogue] = [this]()
		{
			// WIP
			_eventToRead = Movil;
			startDialogue("Movil");
		};

	roomEvent[ResolveBottons] = [this]() //Resolve the case
	{
		entityManager->setActive(rmObjects.posibleCaseButton, true);
		entityManager->setActive(rmObjects.imposibleCaseButton, true);
	};
}

void Room1Scene::_setGlobalFeatures()
{
	//Register scene in dialogue manager
	dialogueManager->setScene(this);
}

void Room1Scene::_setRoomAudio()
{
	//Audio sfx 
	AudioManager& audioMngr = AudioManager::Instance();

	rmSounds.uiButton = sdlutils().soundEffects().at("boton");
	audioMngr.setVolume(rmSounds.uiButton, 0.2);

	rmSounds.puzzleButton = sdlutils().soundEffects().at("puzzle");
	audioMngr.setVolume(rmSounds.puzzleButton, 0.3);

	Sound doorSound = sdlutils().soundEffects().at("puerta");

	//Audio music
	Sound room1music = sdlutils().musics().at("room1music");
	audioMngr.setLooping(room1music, true);
	audioMngr.playSound(room1music);
}

void Room1Scene::_setDialog()
{
	// Dialog
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, _eventToRead);

	assert(rmObjects.quitButton != nullptr); // UI must be Initialized First

	Area2D* quitButtonArea = entityManager->getComponent<Area2D>(rmObjects.quitButton);

	auto dialogEnts = entityManager->getEntities(ecs::grp::DIALOGUE);

	for (ecs::entity_t dialogEnt : dialogEnts)
	{
		Area2D* dialogArea = entityManager->getComponent<Area2D>(dialogEnt);
		if(dialogArea != nullptr)
			areaLayerManager->sendAfter(quitButtonArea->getLayerPos(), dialogArea->getLayerPos());
	}
}

void Room1Scene::_setUI()
{
	// Corpse zoom Quit Button
	rmObjects.quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
	
	entityManager->getComponent<ClickComponent>(rmObjects.quitButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
		{
			AudioManager::Instance().playSound(rmSounds.uiButton);
			entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
			entityManager->setActive(rmObjects.quitButton, false);
			entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
		});

	entityManager->setActive(rmObjects.quitButton, false);

	// Pause Button
	rmObjects.pauseButton = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	
	entityManager->getComponent<ClickComponent>(rmObjects.pauseButton)
		->connect(ClickComponent::JUST_CLICKED, [this]() 
		{
			AudioManager::Instance().playSound(rmSounds.uiButton);
		});

	//Inventory
	auto InventoryBackground = entityFactory->CreateImageEntity(entityManager, "fondoPruebaLog", Vector2D(1050, 0), Vector2D(0, 0), 300, 1500, 0, ecs::grp::UI);
	
	rmObjects.inventoryButton = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(InventoryBackground, false);

	auto InvArea = entityManager->addComponent<RectArea2D>(InventoryBackground, areaLayerManager);

	auto inventoryUpButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 70), Vector2D(0, 0), 70, 70, -90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(inventoryUpButton, false);

	auto inventoryDownButton = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1170, 748 - 268 / 3 - 20), Vector2D(0, 0), 70, 70, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	entityManager->setActive(inventoryDownButton, false);

	entityManager->getComponent<ClickComponent>(rmObjects.inventoryButton)
		->connect(ClickComponent::JUST_CLICKED, [this, InventoryBackground, inventoryUpButton, inventoryDownButton, InvArea]()
		{
			AudioManager::Instance().playSound(rmSounds.uiButton);
			GetInventory()->setActive(!GetInventory()->getActive());  //Toggle the inventory

			if (GetInventory()->getActive()) // If the inventory is active, activate the items
			{
				entityManager->setActive(InventoryBackground, true);

				entityManager->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(925);

				//change the position of the log button
				areaLayerManager->sendFront(InvArea->getLayerPos());

				areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inventoryUpButton)->getLayerPos());
				areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(inventoryDownButton)->getLayerPos());

				entityManager->setActive(inventoryDownButton, true);
				entityManager->setActive(inventoryUpButton,   true);

				for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i)
					GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], true);  // Activate the items
			}
			else 
			{
				entityManager->setActive(InventoryBackground, false);
				entityManager->setActive(inventoryDownButton, false);
				entityManager->setActive(inventoryUpButton,   false);

				rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);

				// its okay to use the first item as the first item to show??
				for (int i = GetInventory()->getFirstItem(); i < GetInventory()->getItemNumber(); ++i)
					GetInventory()->hints[i]->getMngr()->setActive(GetInventory()->hints[i], false);  // Desactivate the hints
			}
		});

	entityManager->getComponent<ClickComponent>(inventoryDownButton)
		->connect(ClickComponent::JUST_CLICKED, [this]() 
		{
			AudioManager::Instance().playSound(rmSounds.uiButton);
			scrollInventory(1);
		});

	// Button that confirms the assesination
	rmObjects.posibleCaseButton = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3) - 20), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	
	entityManager->getComponent<ClickComponent>(rmObjects.posibleCaseButton)
		->connect(ClickComponent::JUST_CLICKED, [this]() 
		{ 
			roomEvent[GoodEnd](); 
		});

	entityManager->setActive(rmObjects.posibleCaseButton, false);

	// Button that confirms the assesination is not possible
	rmObjects.imposibleCaseButton = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3)), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
	
	entityManager->getComponent<ClickComponent>(rmObjects.imposibleCaseButton)
		->connect(ClickComponent::JUST_CLICKED, [this]() 
		{
			roomEvent[BadEnd](); 
		});

	entityManager->setActive(rmObjects.imposibleCaseButton, false);

	Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager);
}

void Room1Scene::_setRoomBackground()
{
	auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1160 - 1349, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));

	auto StudyRoomBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(StudyRoomBackground));

	//StudyRoom (Right)
	auto LivingRoomBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(-1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingRoomBackground));

	entityManager->getComponent<ClickComponent>(ChangeRoom1)
		->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoomScroll]() 
		{
			if (!rmObjects.backgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				rmObjects.backgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
		});

	entityManager->getComponent<ClickComponent>(ChangeRoom2)
		->connect(ClickComponent::JUST_CLICKED, [this]() 
		{
			if (!rmObjects.backgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				rmObjects.backgroundScroll->Scroll(ScrollComponent::LEFT);
			}
		});
}

void Room1Scene::_setCaseResolution()
{
	//set the scene the variant is 
	Game::Instance()->getDataManager()->SetSceneCount(ROOM1);
	
	//get actual variant
	int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(ROOM1);

	auto possibleButton = entityFactory->CreateInteractableEntity(entityManager, "posible", EntityFactory::RECTAREA, Vector2D(500, 0), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	entityManager->setActive(possibleButton, false);

	entityManager->getComponent<ClickComponent>(possibleButton)
		->connect(ClickComponent::JUST_CLICKED, [this, variantAct]() 
		{
			if (variantAct != 0) //if its the not correct variant one dies
			{
				Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
				std::cout << "NO CORRECTA DE POS" << std::endl;
			}

			//change to intermediate room
		});

	auto noPossibleButton = entityFactory->CreateInteractableEntity(entityManager, "noPosible", EntityFactory::RECTAREA, Vector2D(600, 0), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	entityManager->setActive(noPossibleButton, false);
	
	entityManager->getComponent<ClickComponent>(noPossibleButton)
		->connect(ClickComponent::JUST_CLICKED, [this, variantAct]() 
		{
			if (variantAct != 1 || 2) //if its the not correct variant one dies
			{
				std::cout << "NO CORRECTA DE NO POS" << std::endl;
				Game::Instance()->getDataManager()->SetCharacterDead(KEISARA);
			}

			//change to intermediate room
		});

	auto resolveButton = entityFactory->CreateInteractableEntity(entityManager, "resolve", EntityFactory::RECTAREA, Vector2D(0, 500), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	entityManager->setActive(resolveButton, false);

	auto noResolveButton = entityFactory->CreateInteractableEntity(entityManager, "noResolve", EntityFactory::RECTAREA, Vector2D(0, 600), Vector2D(0, 0), 500, 500, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);
	entityManager->setActive(noResolveButton, false);

	//Button only appears when the 3 puzzles have been resolved
	auto readyToResolveButton = entityFactory->CreateInteractableEntity(entityManager, "B5", EntityFactory::RECTAREA, Vector2D(400, 400), Vector2D(0, 0), 400, 400, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::ZOOMOBJ);

	entityManager->getComponent<ClickComponent>(readyToResolveButton)
		->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToResolveButton]() 
		{
			entityManager->setActive(noResolveButton, true);
			entityManager->setActive(resolveButton, true);
			entityManager->setActive(readyToResolveButton, false);
		});

	entityManager->setActive(readyToResolveButton, false);

	entityManager->getComponent<ClickComponent>(resolveButton)
		->connect(ClickComponent::JUST_CLICKED, [this, possibleButton, noPossibleButton, readyToResolveButton, resolveButton, noResolveButton]() 
		{
			entityManager->setActive(noPossibleButton, true);

			entityManager->setActive(readyToResolveButton, false);
			entityManager->setActive(possibleButton, true);

			entityManager->setActive(resolveButton, false);
			entityManager->setActive(noResolveButton, false);
		});

	entityManager->getComponent<ClickComponent>(noResolveButton)
		->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToResolveButton]() 
		{
			entityManager->setActive(noResolveButton, false);
			entityManager->setActive(resolveButton, false);
			entityManager->setActive(readyToResolveButton, true);
		});
}

void Room1Scene::_setInteractuables()
{
	// Corpse Clue
	auto corpseZoom = entityFactory->CreateImageEntity(entityManager, "CorspeBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);

	RectArea2D* corpseZoomArea = entityManager->addComponent<RectArea2D>(corpseZoom, areaLayerManager);
	entityManager->setActive(corpseZoom, false);

	characterCorpse = entityFactory->CreateInteractableEntity(entityManager, "Corspe", EntityFactory::RECTAREA, Vector2D(1000, 422), Vector2D(0, 0), 268, 326, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(characterCorpse));

	entityManager->getComponent<ClickComponent>(characterCorpse)
		->connect(ClickComponent::JUST_CLICKED, [this, corpseZoom]()
			{
				entityManager->setActive(corpseZoom, true);
				entityManager->setActive(rmObjects.quitButton, true);

				if (!finishallpuzzles)roomEvent[CorpseDialogue]();
				else roomEvent[ResolveBottons]();
			});

	//Mobile Clue
	auto Mobile = entityFactory->CreateInteractableEntity(entityManager, "mobileSprite", EntityFactory::RECTAREA, Vector2D(1250, 500), Vector2D(0, 0), 245 / 3, 123 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Mobile));

	auto mobileZoom = entityFactory->CreateImageEntity(entityManager, "mobileBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);

	RectArea2D* mobileZoomArea = entityManager->addComponent<RectArea2D>(mobileZoom, areaLayerManager);
	entityManager->setActive(mobileZoom, false);

	entityManager->getComponent<ClickComponent>(Mobile)
		->connect(ClickComponent::JUST_CLICKED, [this, mobileZoom, Mobile]()
			{
				entityManager->setActive(mobileZoom, true);
				entityManager->setActive(rmObjects.quitButton, true);
				roomEvent[MobileDialogue]();
			});

	// Timetable Clue
	auto Timetable = entityFactory->CreateInteractableEntity(entityManager, "Timetable", EntityFactory::RECTAREA, Vector2D(1173, 267), Vector2D(0, 0), 138, 182, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Timetable));

	auto _calendearZoom = entityFactory->CreateImageEntity(entityManager, "TimetableZoom", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	entityManager->setActive(_calendearZoom, false);

	entityManager->getComponent<ClickComponent>(Timetable)
		->connect(ClickComponent::JUST_CLICKED, [this, Timetable, _calendearZoom]()
			{
				//this->startDialogue(Calendario);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
				Timetable->getMngr()->setActive(Timetable, false);
				entityManager->setActive(_calendearZoom, true);
				entityManager->setActive(rmObjects.quitButton, true);
			});

	// Puzzles

	auto teaCupPzButton = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSprite", EntityFactory::RECTAREA, Vector2D(700, 265), Vector2D(0, 0), 165 / 3, 176 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(teaCupPzButton));

	entityManager->getComponent<ClickComponent>(teaCupPzButton)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				roomEvent[TeaCupPuzzleSnc]();
			});


	auto Clock = entityFactory->CreateInteractableEntity(entityManager, "Clock", EntityFactory::RECTAREA, Vector2D(828, 95), Vector2D(0, 0), 142, 553, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Clock));

	entityManager->getComponent<ClickComponent>(Clock)->connect(ClickComponent::JUST_CLICKED, [this]()
		{
			AudioManager::Instance().playSound(rmSounds.puzzleButton);
			roomEvent[ClockPuzzleSnc]();
		});

	auto Shelf = entityFactory->CreateInteractableEntity(entityManager, "Shelf", EntityFactory::RECTAREA, Vector2D(214, 96), Vector2D(0, 0), 191, 548, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Shelf));

	entityManager->getComponent<ClickComponent>(Shelf)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				roomEvent[BooksPuzzleScn]();
			});

	// (Left part of living room)

	auto Tubes = entityFactory->CreateInteractableEntity(entityManager, "Tubes", EntityFactory::RECTAREA, Vector2D(356 - 1349 - 6, 127), Vector2D(0, 0), 616, 336, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Tubes));

	entityManager->getComponent<ClickComponent>(Tubes)
		->connect(ClickComponent::JUST_CLICKED, [this]()
			{
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				roomEvent[PipePuzzleSnc]();
			});

	//Spoon
	auto spoonObject = entityFactory->CreateInteractableEntity(entityManager, "SceneSpoon", EntityFactory::RECTAREA, Vector2D(275 - 1349 - 6, 540), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(spoonObject));

	entityManager->getComponent<ClickComponent>(spoonObject)
		->connect(ClickComponent::JUST_CLICKED, [this, spoonObject]()
			{
				entityManager->setActive(spoonObject, false);
				roomEvent[Spoon]();
			});

	areaLayerManager->sendFront(mobileZoomArea->getLayerPos());
	areaLayerManager->sendFront(corpseZoomArea->getLayerPos());
}