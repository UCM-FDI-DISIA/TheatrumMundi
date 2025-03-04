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
#include "../../TheatrumMundiProyect/TheatrumMundi/EntityFactory.h"


Room1Scene::Room1Scene(): SceneRoomTemplate()
{
	roomEvent.resize(event_size);
	roomEvent[InitialDialogue] = [this] 
	{
		startDialogue(SalaIntermedia1);
	};
	roomEvent[CorpseDialogue] = [this]
		{
#ifdef DEBUG
			std::cout << "semurio";
#endif // DEBUG

			
			startDialogue(SalaIntermedia1);
		};
	roomEvent[PipePuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(PIPE_PUZZLE,this);
		};
	roomEvent[PipePuzzleRsv] = [this] {
		// InventoryLogic
		
		};
	roomEvent[BooksPuzzleScn] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(BOOKS_PUZZLE, this);
		};
	roomEvent[BooksPuzzleRsv] = [this] {
		// InventoryLogic
		
		};
	roomEvent[ClockPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(CLOCK_PUZZLE, this);
		};
	roomEvent[ClockPuzzleRsv] = [this] {
		// InventoryLogic
		
		};
	roomEvent[TeaCupPuzzleSnc] = [this]
		{
			Game::Instance()->getSceneManager()->loadScene(TEA_CUP_PUZZLE, this);
		};
	roomEvent[TeaCupPuzzleRsv] = [this] {
		// InventoryLogic
		entityManager->removeComponent<ClickComponent>(puzzleptr[4]);
		};
	roomEvent[Spoon] = [this] {
		// InventoryLogic
		};
	roomEvent[ResolveCase] = [this] {
		//Poner el dialogo correspondiente
		startDialogue(SalaIntermedia1);
		};
	roomEvent[GoodEnd] = [this] {
		// WIP
		};
	roomEvent[BadEnd] = [this] {
		// WIP
		};
	
}

Room1Scene::~Room1Scene()
{
}

void Room1Scene::init()
{
 
	if (!isStarted) {
		isStarted = true;
		finishalpuzzles = false;
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		a.setVolume(buttonSound, 0.2);
		Sound puzzleButtonSound = sdlutils().soundEffects().at("puzzle");
		a.setVolume(puzzleButtonSound, 0.3);
		Sound doorSound = sdlutils().soundEffects().at("puerta");
		//Audio music
		Sound room1music = sdlutils().musics().at("room1music");
		a.setLooping(room1music, true);
		a.playSound(room1music);

		//Register scene in dialogue manager
		Game::Instance()->getDialogueManager()->setScene(this);

		//CharacterImage
		//auto characterimg = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0, ecs::grp::DIALOGUE);
		auto characterimg = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(characterimg, Vector2D(1349 / 5, 748 / 8), Vector2D(0, 0), 2019/2, 1122/2, 0);
		auto imCh = entityManager->addComponent<Image>(characterimg, &sdlutils().images().at("Dialog"));
		
		Game::Instance()->getDialogueManager()->setCharacterImg(imCh);
		entityManager->setActive(characterimg, false);

		/*
		//All Screen: Object to detect click on screen. Used to read displayed dialogue.
		auto _screenDetect = entityManager->addEntity(ecs::grp::DIALOGUE);
		entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->setActive(_screenDetect, false);
		*/

		//Create dialogue text entity. Object that renders dialogue Text on Screen
		auto _textbackground = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(_textbackground, Vector2D(-200, 0), Vector2D(0, 0),1349,748, 0);
		entityManager->addComponent<Image>(_textbackground, &sdlutils().images().at("Dialog"));
		entityManager->addComponent<RectArea2D>(_textbackground,areaLayerManager);

		entityManager->addComponent<ClickComponent>(_textbackground)->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
			{
				if (!logActive) {
					//read dialogue only if it has to
					if (Game::Instance()->getDialogueManager()->getDisplayOnProcess())
					{
						Game::Instance()->getDialogueManager()->ReadDialogue(SalaIntermedia1);
					}
					else
					{
						_textbackground->getMngr()->setActive(_textbackground, false);
					}
				}
			});
		entityManager->addComponent<TriggerComponent>(_textbackground);
		entityManager->setActive(_textbackground, false);


		auto _textTest = entityManager->addEntity(ecs::grp::DIALOGUE);
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		entityManager->setActive(_textTest, false);



		//Create log
		auto _log = entityManager->addEntity(ecs::grp::UI);
		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog"), 200); //background log

		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent

		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog =
			entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("BASE"), colorText, logComp->getLogList()); //write text component

		_log->getMngr()->setActive(_log, false); //hide log at the beggining

		//Register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(logComp);
		//Game::Instance()->getDialogueManager()->setScene(this);

		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest,sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

		Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);

		roomEvent[LOGENABLE] = [this] {
			//activate log
			/*_log->getMngr()->setActive(_log, true);
			logActive = true;

			//activate close log button
			_closeLogButton->getMngr()->setActive(_closeLogButton, true);

			//disable open log button
			_openLogButton->getMngr()->setActive(_openLogButton, false);
			*/
			};
		roomEvent[LOGDESABLE] = [this, _log] {
			//disable log
			_log->getMngr()->setActive(_log, false);
			logActive = false;

			//activate open log button
			//_openLogButton->getMngr()->setActive(_openLogButton, true);

			//disable close log button
			//_closeLogButton->getMngr()->setActive(_closeLogButton, false);
			};


		auto ChangeRoom1 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(34, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width())/12) /*- 1*/, EntityFactory::SCROLLNORMAL, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoom2 = entityFactory->CreateInteractableEntityScroll(entityManager, "ChangeRoom", EntityFactory::RECTAREA, Vector2D(1160 - 1349, 160), Vector2D(0, 0), 136, 495, 0, areaLayerManager, 12, ((sdlutils().width()) / 12) /*- 1*/, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		auto ChangeRoomScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		ChangeRoomScroll->addElementToScroll(entityManager->getComponent<Transform>(ChangeRoom2));
		auto _quitButton = entityFactory->CreateInteractableEntity(entityManager, "B1", entityFactory->RECTAREA, Vector2D(1349 - 110, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, entityFactory->NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(_quitButton)->connect(ClickComponent::JUST_CLICKED, [this, _quitButton,buttonSound]()
			{
				AudioManager::Instance().playSound(buttonSound);
				entityManager->setActiveGroup(ecs::grp::ZOOMOBJ, false);
				entityManager->setActive(_quitButton, false);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, true);
			});
		entityManager->setActive(_quitButton, false);
		auto StudyBackground = entityFactory->CreateImageEntity(entityManager, "StudyBackground", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		auto StudyBackgroundScroll = entityManager->getComponent<ScrollComponent>(ChangeRoom1);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(StudyBackground));

		//StudyRoom (Right)
		auto LivingBackground = entityFactory->CreateImageEntity(entityManager, "LivingroomBackground", Vector2D(- 1349-6, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::DEFAULT);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(LivingBackground));

		auto _corpseZoom = entityFactory->CreateImageEntity(entityManager, "CorspeBackground", Vector2D(0, 0), Vector2D(0, 0), 1349,748, 0, ecs::grp::ZOOMOBJ);
		entityManager->setActive(_corpseZoom, false);

		auto Corspe = entityFactory->CreateInteractableEntity(entityManager, "Corspe",EntityFactory::RECTAREA, Vector2D(1000, 422), Vector2D(0, 0), 268, 326, 0, areaLayerManager,EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Corspe));
		entityManager->getComponent<ClickComponent>(Corspe)->connect(ClickComponent::JUST_CLICKED, [this,_corpseZoom,_quitButton]() {
			_corpseZoom->getMngr()->setActive(_corpseZoom, true);
			entityManager->setActive(_quitButton, true);
			if (!finishalpuzzles)roomEvent[CorpseDialogue]();
			else roomEvent[ResolveBottons]();

		});


		//Mobile

		auto Mobile = entityFactory->CreateInteractableEntity(entityManager, "mobile", EntityFactory::RECTAREA, Vector2D(123, 267), Vector2D(0, 0), 128, 82, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Mobile));

		auto _mobileZoom = entityFactory->CreateImageEntity(entityManager, "mobileZoom", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		entityManager->setActive(_mobileZoom, false);
		
		entityManager->getComponent<ClickComponent>(Mobile)->connect(ClickComponent::JUST_CLICKED, [this, _mobileZoom, Mobile, _quitButton]()
			{
				//this->startDialogue(Calendario);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
				entityManager->setActive(_mobileZoom, true);
				entityManager->setActive(_quitButton, true);
			});




		auto Timetable = entityFactory->CreateInteractableEntity(entityManager, "Timetable", EntityFactory::RECTAREA, Vector2D(1173, 267), Vector2D(0, 0), 138, 182, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Timetable));
		
		auto _calendearZoom = entityFactory->CreateImageEntity(entityManager, "calendearZoom", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::ZOOMOBJ);
		entityManager->setActive(_calendearZoom, false);
		
		entityManager->setActive(_quitButton, false);
		entityManager->getComponent<ClickComponent>(Timetable)->connect(ClickComponent::JUST_CLICKED, [this, Timetable,_calendearZoom,_quitButton]()
			{
				//this->startDialogue(Calendario);
				entityManager->setActiveGroup(ecs::grp::INTERACTOBJ, false);
				Timetable->getMngr()->setActive(Timetable, false);
				entityManager->setActive(_calendearZoom,true);
				entityManager->setActive(_quitButton, true);
			});

		//TeaCup
		auto TeaCup = entityFactory->CreateInteractableEntity(entityManager, "Clock", EntityFactory::RECTAREA, Vector2D(500, 200), Vector2D(0, 0), 50, 50, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(TeaCup));
		entityManager->getComponent<ClickComponent>(TeaCup)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[TeaCupPuzzleSnc](); 
			});


		auto Clock = entityFactory->CreateInteractableEntity(entityManager, "Clock", EntityFactory::RECTAREA, Vector2D(828, 95), Vector2D(0, 0), 142, 553, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Clock));
		entityManager->getComponent<ClickComponent>(Clock)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[ClockPuzzleSnc]();
			});

		auto Shelf = entityFactory->CreateInteractableEntity(entityManager, "Shelf", EntityFactory::RECTAREA, Vector2D(214, 96), Vector2D(0, 0), 191, 548, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Shelf));
		entityManager->getComponent<ClickComponent>(Shelf)->connect(ClickComponent::JUST_CLICKED, [this, puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[BooksPuzzleScn]();
			});
		//LivingRoom (Left)

		auto Tubes = entityFactory->CreateInteractableEntity(entityManager, "Tubes", EntityFactory::RECTAREA, Vector2D(356-1349 - 6, 127), Vector2D(0, 0), 616, 336, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::INTERACTOBJ);
		StudyBackgroundScroll->addElementToScroll(entityManager->getComponent<Transform>(Tubes));
		entityManager->getComponent<ClickComponent>(Tubes)->connect(ClickComponent::JUST_CLICKED, [this,puzzleButtonSound]() {
			AudioManager::Instance().playSound(puzzleButtonSound);
			roomEvent[PipePuzzleSnc]();
			});

		auto ChangeRoom1Button = entityManager->getComponent<ClickComponent>(ChangeRoom1);
		ChangeRoom1Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom1Button,StudyBackgroundScroll, doorSound]() {
			if (!StudyBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				StudyBackgroundScroll->Scroll(ScrollComponent::RIGHT);
			}
			});

		auto ChangeRoom2Button = entityManager->getComponent<ClickComponent>(ChangeRoom2);
		ChangeRoom2Button->connect(ClickComponent::JUST_CLICKED, [this, ChangeRoom2Button, StudyBackgroundScroll, doorSound]() {
			if (!StudyBackgroundScroll->isScrolling()) {
				AudioManager::Instance().playSound(doorSound);
				StudyBackgroundScroll->Scroll(ScrollComponent::LEFT);
			}
			});
		
		//UI
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager,"B3", EntityFactory::RECTAREA ,Vector2D(20,20), Vector2D(0,0), 90,90,0,areaLayerManager,EntityFactory::NODRAG ,ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});

		auto buttonInventory = entityFactory->CreateInteractableEntity(entityManager, "B2", EntityFactory::RECTAREA, Vector2D(40 + 268 / 3 ,20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonInventoryClick = entityManager->getComponent<ClickComponent>(buttonInventory);
		buttonInventoryClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});

		auto buttonLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(20, 748 - (268/3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		
		auto buttonCloseLog = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 500), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		
		ClickComponent* buttonLogClick = entityManager->getComponent<ClickComponent>(buttonLog);
		buttonLogClick->connect(ClickComponent::JUST_CLICKED, [this, _log, buttonLog, buttonCloseLog, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			//open log
			entityManager->setActive(_log, true);
			logActive = true;
			
			entityManager->setActive(buttonCloseLog, true);
			entityManager->setActive(buttonLog, false);
			});

		entityManager->setActive(buttonLog, true);
		
		ClickComponent* buttonCloseLogClick = entityManager->getComponent<ClickComponent>(buttonCloseLog);
		buttonCloseLogClick->connect(ClickComponent::JUST_CLICKED, [this, _log, buttonLog, buttonCloseLog, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			//open log
			entityManager->setActive(_log, false);
			logActive = false;
			entityManager->setActive(buttonLog, true);
			entityManager->setActive(buttonCloseLog, false);

			});
		entityManager->setActive(buttonCloseLog, false);

		auto buttonpos = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3) - 20), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(buttonpos)->connect(ClickComponent::JUST_CLICKED, [this]() { roomEvent[GoodEnd]();});
		entityManager->setActive(buttonpos,false);

		auto buttonimp = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(750, 748 - (268 / 3)), Vector2D(0, 0), 268 / 3, 268 / 3, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		entityManager->getComponent<ClickComponent>(buttonimp)->connect(ClickComponent::JUST_CLICKED, [this]() { roomEvent[BadEnd]();});
		entityManager->setActive(buttonimp, false);
		//Resolve the case
		roomEvent[ResolveBottons] = [this, buttonpos, buttonimp]() {
			entityManager->setActive(buttonpos, true);
			entityManager->setActive(buttonimp, true);
			};


		//X Button "B1"
	}
	SDL_Delay(1000);

	//roomEvent[InitialDialogue]();
	
}

void Room1Scene::resolvedPuzzle(int i)
{
	if (i == ClockPuzzleRsv || i == PipePuzzleRsv || i == BooksPuzzleRsv || i == TeaCupPuzzleRsv) {
		roomEvent[i]();
		bool aux = true;
		for (bool a : puzzlesol) if (!a) aux = false;
		finishalpuzzles = aux;
		if (aux) entityManager->setActive(body, true);
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
