  #include "Room1.h"
#include <list>
#include "DataManager.h"
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
#include "InvAnimComponent.h"
#include "../TheatrumMundi/EntityFactory.h"
#include "EventsInfo.h"
#include "Log.h"

#include "PauseManager.h"


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
	stopAnimation = false;

	_setRoomAudio();

	_setGlobalFeatures();

	_setRoomBackground();

	_setInteractuables();

	_setUI();

	_setDialog();

	_setCaseResolution();

	roomEvent[InitialDialogue]();

	_loadimg1 = entityFactory->CreateImageEntity(entityManager, "loading1", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg1->getMngr()->setActive(_loadimg1, false);

	_loadimg2 = entityFactory->CreateImageEntity(entityManager, "loading2", Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0, ecs::grp::DECISION);
	_loadimg2->getMngr()->setActive(_loadimg2, false);

	SDL_Delay(1000);
}



void Room1Scene::resolvedPuzzle(int i)
{
	puzzlesol[i] = true;
	//entityManager->removeComponent<ClickComponent>(puzzleptr[i]);
	if (i <4) {
		//int auxevent = EVENTS_SIZE;
		//if (i == 0)  auxevent = PipePuzzleRsv;
		//else if (i == 1)  auxevent = BooksPuzzleRsv;
		//else if (i == 2)  auxevent = ClockPuzzleRsv;
		//else if (i == 3)  auxevent = TeaCupPuzzleRsv;
		//roomEvent[auxevent]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishallpuzzles = aux;
		if (aux) entityManager->setActive(characterCorpse, true);
	}
	else {
#ifdef _



		std::cout << i << " invalid index" << std::endl;
#endif
	}


	if (finishallpuzzles)
	{
		//call to the final dialogue
		roomEvent[ResolveCase]();
		//
		
	}
}
void Room1Scene::unload()
{
	entityManager->~EntityManager();
}

//// Private Internal Setting Methods



void Room1Scene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);

	
	entityManager->setActiveGroup(ecs::grp::MIDDLEROOM, false);

	if (finishallpuzzles)
	{
		roomEvent[ResolveBottons]();
	}

}

void Room1Scene::_setRoomEvents()
{
	roomEvent.resize(EVENTS_SIZE);

	roomEvent[InitialDialogue] = [this]()
		{ 
			startDialogue("Sala1Intro"); 
		};
	
	roomEvent[CorpseDialogue] = [this]()
		{
			_eventToRead = Cadaver;
			startDialogue("Cadaver");
		};

	roomEvent[PipePuzzleSnc] = [this]()
		{
			HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
			rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
			Game::Instance()->getSceneManager()->loadScene(PIPE_PUZZLE, this);
		};

	roomEvent[BooksPuzzleScn] = [this]()
		{
			HideAllInvetoryItems(invObjects.InventoryBackground,invObjects.inventoryUpButton,invObjects.inventoryDownButton);
			rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
			Game::Instance()->getSceneManager()->loadScene(BOOKS_PUZZLE, this);
		};



	roomEvent[ClockPuzzleSnc] = [this]()
		{
			HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
			rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};



	roomEvent[TeaCupPuzzleSnc] = [this]()
		{
			HideAllInvetoryItems(invObjects.InventoryBackground, invObjects.inventoryUpButton, invObjects.inventoryDownButton);
			rmObjects.inventoryButton->getMngr()->getComponent<Transform>(rmObjects.inventoryButton)->setPosX(60 + 268 / 3);
			Game::Instance()->getSceneManager()->loadScene(TEA_CUP_PUZZLE, this);
		};


	roomEvent[Spoon] = [this]()
		{
			inv->addItem(new Hint("TeaCupSpoon", sdlutils().invDescriptions().at("TeaCupSpoon"), &sdlutils().images().at("TeaCupSpoon")));
			inv->hints.push_back(entityFactory->CreateInvEntity(entityManager, "TeaCupSpoon", EntityFactory::RECTAREA, inv->setPosition(), Vector2D(0, 0), 100, 100, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI));
			createDescription(inv->hints.back(), inv->getItems().back());
			if(inv->getActive()) inv->hints.back()->getMngr()->setActive(inv->hints.back(), true);
			else inv->hints.back()->getMngr()->setActive(inv->hints.back(), false);
		};
			
	roomEvent[ResolveCase] = [this]() {
		startDialogue("Sala1Final");
		roomEvent[ResolveBottons]();

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
			

	roomEvent[GoodEnd] = [this]()
		{
			// black background
			Game::Instance()->getDataManager()->SetSceneCount(SceneCount::MIDDLEROOM2);
			_loadimg1->getMngr()->setActive(_loadimg1, true);
			entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
			std::shared_ptr<Sound> correctSound = sdlutils().soundEffects().at("correcto");
			AudioManager::Instance().playSound(correctSound);
			Game::Instance()->render();
			Game::Instance()->getSceneManager()->popScene();
		};
	roomEvent[BadEnd] = [this]()
		{
			// black background
			Game::Instance()->getDataManager()->SetCharacterDead(Character::KEISARA);
			Game::Instance()->getDataManager()->SetSceneCount(SceneCount::MIDDLEROOM2);
			_loadimg2->getMngr()->setActive(_loadimg2, true);
			entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
			std::shared_ptr<Sound> incorrectSound = sdlutils().soundEffects().at("incorrecto");
			AudioManager::Instance().playSound(incorrectSound);
			Game::Instance()->render();
			Game::Instance()->getSceneManager()->popScene();
		};

	roomEvent[MobileDialogue] = [this]()
		{
			int variant = Game::Instance()->getDataManager()->GetRoomVariant(0);

			if (variant == 0 || variant == 2)
			{
				//_eventToRead = Movil;
				startDialogue("Movil1");
			}
			if (variant == 1)
			{
				//_eventToRead = Movil;
				startDialogue("Movil2");
			}
			
		};

	roomEvent[ResolveBottons] = [this]() //Resolve the case
	{
			entityManager->setActive(rmObjects.readyToResolveBotton, true);
	

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

	rmSounds.doorSound = sdlutils().soundEffects().at("puerta");

	audioMngr.stopSound(sdlutils().musics().at("intermedia"));
	std::shared_ptr<Sound> room1music = sdlutils().musics().at("sala1");
	audioMngr.playSound(room1music, true);

}

void Room1Scene::_setDialog()
{
	// Dialog
	dialogueManager->Init(0, entityFactory, entityManager, false, areaLayerManager, _eventToRead);

	assert(rmObjects.inventoryButton != nullptr); // UI must be Initialized First

	Area2D* inventoryButtonArea = entityManager->getComponent<Area2D>(rmObjects.inventoryButton);

	auto dialogEnts = entityManager->getEntities(ecs::grp::DIALOGUE);

	for (ecs::entity_t dialogEnt : dialogEnts)
	{
		Area2D* dialogArea = entityManager->getComponent<Area2D>(dialogEnt);
		if(dialogArea != nullptr)
			areaLayerManager->sendAfter(inventoryButtonArea->getLayerPos(), dialogArea->getLayerPos());
	}
}

struct TimerData {
	EntityManager* manager;
	PauseManager* pauseM;
};

Uint32 timerCallbackRoom1(Uint32 interval, void* param) {

	auto data = static_cast<TimerData*>(param);

	data->manager->setActive(data->pauseM->_getopenPauseButton(), true);

	delete data;
	return 0;
}




void Room1Scene::_setUI()
{
	// Corpse zoom Quit Button
	rmObjects.quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
	



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
			
			TimerData* t = new TimerData{ entityManager,pauseManager };
			SDL_AddTimer(50, timerCallbackRoom1, t);
			

		});

	entityManager->setActive(rmObjects.quitButton, false);

	
	//inventory descriptions
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
				entityManager->setActive(invObjects.inventoryUpButton,   true);

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
				
				for (int i = inv->getFirstItem(); i < inv->getItemNumber() + inv->getFirstItem(); ++i) 
					inv->hints[i]->getMngr()->getComponent<InvAnimComponent>(inv->hints[i])->endInvAnim();// Desactivate the items 
				
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

	// Pause Logic
	pauseManager->setScene(this);
	pauseManager->Init(entityFactory,entityManager,areaLayerManager);

	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getbackgroundNotInteractable())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getreanudePauseButton())->getLayerPos());
	areaLayerManager->sendFront(entityManager->getComponent<RectArea2D>(pauseManager->_getexitPauseButton())->getLayerPos());

	logbtn = rmObjects.logbtn = Game::Instance()->getLog()->Init(entityFactory, entityManager, areaLayerManager,this);
}

void Room1Scene::_setRoomBackground()
{
	auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "estudiopuerta", EntityFactory::RECTAREA, Vector2D(13, 142), Vector2D(0, 0), 170, 575, 0, areaLayerManager, 12, ((sdlutils().width()) / 12.0) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "salonpuerta", EntityFactory::RECTAREA, Vector2D(1161 - 1349, 138), Vector2D(0, 0), 175, 575, 0, areaLayerManager, 12, ((sdlutils().width()) / 12.0) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	
	
	auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom1));
	ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));

	ChangeRoomScroll->setEndScrollCallback([this]() {
		scrolling = false; 
		});
	entityManager->getComponent<ScrollComponent>(ChangeRoom2)->setEndScrollCallback([this]() {
		scrolling = false;
		});



	auto StudyRoomBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(StudyRoomBackground));
	//StudyRoom (Right)
	auto LivingRoomBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(-1349 - 6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingRoomBackground));

	entityManager->getComponent<ClickComponent>(ChangeRoom1)
		->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoomScroll, ChangeRoom2]() 
		{
			if (!rmObjects.backgroundScroll->isScrolling()) {
				if (rmObjects.backgroundScroll->Scroll(ScrollComponent::RIGHT)) {
					auto trChangeRoom2 = entityManager->getComponent<Transform>(ChangeRoom2);
					trChangeRoom2->setPos(Vector2D(1161 - 1349, 138));
					AudioManager::Instance().playSound(rmSounds.doorSound); //If you can scroll, scroll and play the door sound
					scrolling = true;
				}
			}
		});

	entityManager->getComponent<ClickComponent>(ChangeRoom2)
		->connect(ClickComponent::JUST_CLICKED, [this,ChangeRoom1]() 
		{
			if (!rmObjects.backgroundScroll->isScrolling()) {
				if (rmObjects.backgroundScroll->Scroll(ScrollComponent::LEFT)) {
					auto trChangeRoom1 = entityManager->getComponent<Transform>(ChangeRoom1);
					trChangeRoom1->setPos(Vector2D(26+1349, 142));
					AudioManager::Instance().playSound(rmSounds.doorSound); //If you can scroll, scroll and play the door sound
					scrolling = true;
				}
			}
		});
}

void Room1Scene::_setCaseResolution()
{

	//set the scene the variant is 
	Game::Instance()->getDataManager()->SetSceneCount(ROOM1);
	
	//get actual variant
	int variantAct = Game::Instance()->getDataManager()->GetRoomVariant(0);
	

	auto background = entityFactory->CreateImageEntity(
		entityManager,"fondoPruebaLog",Vector2D(2, 0),Vector2D(0, 0),1346,748,0,ecs::grp::DECISION);

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
	rmObjects.readyToResolveBotton = entityFactory->CreateInteractableEntity(entityManager, "B5", EntityFactory::RECTAREA, Vector2D(190+ 268 / 3, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

	entityManager->getComponent<ClickComponent>(rmObjects.readyToResolveBotton)
		->connect(ClickComponent::JUST_CLICKED, [this, noResolveButton, resolveButton, readyToQuestion,background]()
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
		->connect(ClickComponent::JUST_CLICKED, [this, resolveButton, noPossibleButton, possibleButton,noResolveButton,readyToQuestion, finalQuestion]()
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
			{if (!scrolling) {
		entityManager->setActive(corpseZoom, true);
		entityManager->setActive(rmObjects.quitButton, true);
		entityManager->setActive(pauseManager->_getopenPauseButton(), false);

		roomEvent[CorpseDialogue]();
	}
			});

	//Mobile Clue
	auto Mobile = entityFactory->CreateInteractableEntity(entityManager, "mobileSprite", EntityFactory::RECTAREA, Vector2D(1250, 500), Vector2D(0, 0), 245 / 3, 123 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Mobile));


	auto mobileZoom = entityFactory->CreateImageEntity(entityManager, "mobileBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);


	int variant = Game::Instance()->getDataManager()->GetRoomVariant(0);
	std::cout << "variant: " << variant << std::endl;
	entity_t tag;
	if (variant == 0 || variant == 2) //call was missed
	{

		entityManager->getComponent<Image>(mobileZoom)->setTexture(&sdlutils().images().at("mobileBackground"));
	}
	else if (variant == 1) // call is answered
	{
		entityManager->getComponent<Image>(mobileZoom)->setTexture(&sdlutils().images().at("mobileBackgroundV2"));
	}

	
	RectArea2D* mobileZoomArea = entityManager->addComponent<RectArea2D>(mobileZoom, areaLayerManager);
	entityManager->setActive(mobileZoom, false);

	entityManager->getComponent<ClickComponent>(Mobile)
		->connect(ClickComponent::JUST_CLICKED, [this, mobileZoom, Mobile]()
			{
				if (!scrolling) {
					auto ImageMobile = entityManager->getComponent<Image>(Mobile);
					ImageMobile->setW(entityManager->getComponent<Transform>(Mobile)->getWidth());
					ImageMobile->setH(entityManager->getComponent<Transform>(Mobile)->getHeight());
					ImageMobile->setPosOffset(0, 0);
					entityManager->setActive(mobileZoom, true);
					entityManager->setActive(rmObjects.quitButton, true);
					auto ImagequitButton = entityManager->getComponent<Image>(rmObjects.quitButton);
					ImagequitButton->setW(entityManager->getComponent<Transform>(rmObjects.quitButton)->getWidth());
					ImagequitButton->setH(entityManager->getComponent<Transform>(rmObjects.quitButton)->getHeight());
					ImagequitButton->setPosOffset(0, 0);
					roomEvent[MobileDialogue]();
					entityManager->setActive(pauseManager->_getopenPauseButton(), false);
				}
			});

	// Timetable Clue
	auto Timetable = entityFactory->CreateInteractableEntity(entityManager, "Timetable", EntityFactory::RECTAREA, Vector2D(1173, 267), Vector2D(0, 0), 138, 182, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Timetable));

	auto _calendearZoom = entityFactory->CreateImageEntity(entityManager, "TimetableZoom", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
	entityManager->setActive(_calendearZoom, false);

	entityManager->getComponent<ClickComponent>(Timetable)
		->connect(ClickComponent::JUST_CLICKED, [this, Timetable, _calendearZoom]()
			{		
		if (!scrolling) {
		//this->startDialogue(Calendario);
		auto ImageTimeTable = entityManager->getComponent<Image>(Timetable);
		ImageTimeTable->setW(entityManager->getComponent<Transform>(Timetable)->getWidth());
		ImageTimeTable->setH(entityManager->getComponent<Transform>(Timetable)->getHeight());
		ImageTimeTable->setPosOffset(0, 0);
		entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
		Timetable->getMngr()->setActive(Timetable, false);
		entityManager->setActive(_calendearZoom, true);
		entityManager->setActive(rmObjects.quitButton, true);
		auto ImagequitButton = entityManager->getComponent<Image>(rmObjects.quitButton);
		ImagequitButton->setW(entityManager->getComponent<Transform>(rmObjects.quitButton)->getWidth());
		ImagequitButton->setH(entityManager->getComponent<Transform>(rmObjects.quitButton)->getHeight());
		ImagequitButton->setPosOffset(0, 0);
		entityManager->setActive(pauseManager->_getopenPauseButton(), false);

			}
			});

	// Puzzles

	auto teaCupPzButton = entityFactory->CreateInteractableEntity(entityManager, "TeaCupSprite", EntityFactory::RECTAREA, Vector2D(700, 265), Vector2D(0, 0), 165 / 3, 176 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(teaCupPzButton));

	entityManager->getComponent<ClickComponent>(teaCupPzButton)
		->connect(ClickComponent::JUST_CLICKED, [this,teaCupPzButton]()
			{
			    auto ImageTeaCup = entityManager->getComponent<Image>(teaCupPzButton);
				ImageTeaCup->setW(entityManager->getComponent<Transform>(teaCupPzButton)->getWidth());
				ImageTeaCup->setH(entityManager->getComponent<Transform>(teaCupPzButton)->getHeight());
				ImageTeaCup->setPosOffset(0, 0);
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				roomEvent[TeaCupPuzzleSnc]();
			});


	auto Clock = entityFactory->CreateInteractableEntity(entityManager, "Clock", EntityFactory::RECTAREA, Vector2D(828, 95), Vector2D(0, 0), 142, 553, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Clock));

	entityManager->getComponent<ClickComponent>(Clock)->connect(ClickComponent::JUST_CLICKED, [this,Clock]()
		{
			auto ImageClock = entityManager->getComponent<Image>(Clock);
			ImageClock->setW(entityManager->getComponent<Transform>(Clock)->getWidth());
			ImageClock->setH(entityManager->getComponent<Transform>(Clock)->getHeight());
			ImageClock->setPosOffset(0, 0);
			AudioManager::Instance().playSound(rmSounds.puzzleButton);
			roomEvent[ClockPuzzleSnc]();
		});

	auto Shelf = entityFactory->CreateInteractableEntity(entityManager, "Shelf", EntityFactory::RECTAREA, Vector2D(214, 96), Vector2D(0, 0), 191, 548, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Shelf));

	entityManager->getComponent<ClickComponent>(Shelf)
		->connect(ClickComponent::JUST_CLICKED, [this,Shelf]()
			{
				auto ImageShelf = entityManager->getComponent<Image>(Shelf);
				ImageShelf->setW(entityManager->getComponent<Transform>(Shelf)->getWidth());
				ImageShelf->setH(entityManager->getComponent<Transform>(Shelf)->getHeight());
				ImageShelf->setPosOffset(0, 0);
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				roomEvent[BooksPuzzleScn]();
			});

	// (Left part of living room)

	auto Tubes = entityFactory->CreateInteractableEntity(entityManager, "Tubes", EntityFactory::RECTAREA, Vector2D(356 - 1349 - 6, 127), Vector2D(0, 0), 616, 336, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Tubes));

	entityManager->getComponent<ClickComponent>(Tubes)
		->connect(ClickComponent::JUST_CLICKED, [this,Tubes]()
			{
				auto ImageTubes = entityManager->getComponent<Image>(Tubes);
				ImageTubes->setW(entityManager->getComponent<Transform>(Tubes)->getWidth());
				ImageTubes->setH(entityManager->getComponent<Transform>(Tubes)->getHeight());
				ImageTubes->setPosOffset(0, 0);
				AudioManager::Instance().playSound(rmSounds.puzzleButton);
				roomEvent[PipePuzzleSnc]();
			});

	//Spoon
	auto spoonObject = entityFactory->CreateInteractableEntity(entityManager, "SceneSpoon", EntityFactory::RECTAREA, Vector2D(275 - 1349 - 6, 540), Vector2D(0, 0), 121 / 3, 105 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);

	rmObjects.backgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(spoonObject));

	entityManager->getComponent<ClickComponent>(spoonObject)
		->connect(ClickComponent::JUST_CLICKED, [this, spoonObject]()
			{
				spoonObject->getMngr()->removeEntity(spoonObject);
				spoonObject->getMngr()->setActive(spoonObject, false);
				roomEvent[Spoon]();
			});

	areaLayerManager->sendFront(mobileZoomArea->getLayerPos());
	areaLayerManager->sendFront(corpseZoomArea->getLayerPos());
}
