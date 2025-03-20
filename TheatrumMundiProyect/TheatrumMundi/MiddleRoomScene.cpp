#include "MiddleRoomScene.h"
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
#include "DataManager.h"
#include "EventsInfo.h"
#include "Log.h"

#include "DialogueManager.h"
#include "../src/components/WriteTextComponent.h"
MiddleRoomScene::MiddleRoomScene() :SceneRoomTemplate(), _eventToRead(SalaIntermedia1)
{
	dialogueManager = new DialogueManager(1);
	roomEvent.resize(MIDDLEROOMEVENTSIZE);
	roomEvent[FIRST_DIALOGUE] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	//ROOM1
	roomEvent[AFTER_ROOM1_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM1_BAD2] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM1_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	//ROOM2
	roomEvent[AFTER_ROOM2_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM2_GOOD2] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM2_BAD2] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM2_BAD1] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	//ROOM3
	roomEvent[AFTER_ROOM3_GOOD3] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_GOOD2SL] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_GOOD2KL] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_BAD2SK] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_BAD1K] = [this]() {
		startDialogue("SalaIntermedia1");
		};
	roomEvent[AFTER_ROOM3_BAD1S] = [this]() {
		startDialogue("SalaIntermedia1");
		};
}

MiddleRoomScene::~MiddleRoomScene()
{
}

void MiddleRoomScene::init()
{
	if (!isStarted) {
		isStarted = true;
		//Audio sfx 
		AudioManager& a = AudioManager::Instance();
		Sound buttonSound = sdlutils().soundEffects().at("boton");
		//Audio music
		Sound room1music = sdlutils().musics().at("room1music");
		a.setLooping(room1music, true);
		a.playSound(room1music);

		//Register scene in dialogue manager
		dialogueManager->setScene(this);
		//MiddleRoomBackground
		entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::MIDDLEROOM);

		
		//CharacterImage
		//auto characterimg = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0, ecs::grp::DIALOGUE);
		
		

		/*Fdua
		//All Screen: Object to detect click on screen. Used to read displayed dialogue.
		auto _screenDetect = entityManager->addEntity(ecs::grp::DIALOGUE);
		entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		entityManager->setActive(_screenDetect, false);
		*/

		//Create dialogue text entity. Object that renders dialogue Text on Screen

		



		//CREATE LOG

		//Log is always created on each middle room scene
		//Register log in dialogue manager

		//background log
		auto _backgroundLog = entityManager->addEntity(ecs::grp::LOG);
		entityManager->addComponent<Transform>(_backgroundLog, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		auto imBack = entityManager->addComponent<Image>(_backgroundLog, &sdlutils().images().at("fondoPruebaLog")); //background log
		//imBack->setAlpha(128);
		entityManager->setActive(_backgroundLog, false);

		//title log
		auto _titleLog = entityManager->addEntity(ecs::grp::LOG);
		entityManager->addComponent<Transform>(_titleLog, Vector2D(50, 50), Vector2D(0, 0), 100, 100, 0);
		entityManager->addComponent<Image>(_titleLog, &sdlutils().images().at("B7"));
		entityManager->setActive(_titleLog, false);

		//scroll log buttons
		auto scrollingLog = entityFactory->CreateInteractableEntityScroll(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1200, 100), Vector2D(0, 0), 50, 50, 270, areaLayerManager, sdlutils().height() / 50, 50, EntityFactory::SCROLLINVERSE, 0, EntityFactory::NODRAG, ecs::grp::LOG);
		auto upScrollingLog = entityFactory->CreateInteractableEntity(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1200, 600), Vector2D(0, 0), 50, 50, 90, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);
		auto ScrollComponentLog = entityManager->getComponent<ScrollComponent>(scrollingLog);
		entityManager->setActive(scrollingLog, false);
		entityManager->setActive(upScrollingLog, false);

		//text log
		auto _textLog = entityManager->addEntity(ecs::grp::LOG);
		Transform* trTextLog = entityManager->addComponent<Transform>(_textLog, Vector2D(0, 0), Vector2D(0, 0), 800, 748, 0);
		Image* imTextLog = entityManager->addComponent<Image>(_textLog, &sdlutils().images().at("fondoPruebaLog"));
		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<std::list<TextInfo>>* writeLog =
			entityManager->addComponent<WriteTextComponent<std::list<TextInfo>>>(_textLog, sdlutils().fonts().at("BASE"), colorText, Game::Instance()->getLog()->getLogList()); //write text component
		entityManager->setActive(_textLog, false);

		ScrollComponentLog->addElementToScroll(entityManager->getComponent<Transform>(_textLog));

		//log buttons
		auto buttonOpenLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(1200, 748 - (268 / 3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		auto buttonCloseLog = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 500), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);



		roomEvent[LOGENABLE] = [this, _backgroundLog, _titleLog, buttonCloseLog, buttonOpenLog, scrollingLog, upScrollingLog] {
			//activate log
			entityManager->setActive(_backgroundLog, true); //background
			entityManager->setActive(_titleLog, true); //title
			entityManager->setActive(buttonCloseLog, true); //close button
			entityManager->setActive(buttonOpenLog, false); //open button
			entityManager->setActive(scrollingLog, true);
			entityManager->setActive(upScrollingLog, true);
			//text log
			//scroll buttons

			/*_log->getMngr()->setActive(_log, true);
			logActive = true;

			//activate close log button
			_closeLogButton->getMngr()->setActive(_closeLogButton, true);

			//disable open log button
			_openLogButton->getMngr()->setActive(_openLogButton, false);
			*/
			};
		roomEvent[LOGDESABLE] = [this, _backgroundLog, _titleLog, buttonCloseLog, buttonOpenLog, scrollingLog, upScrollingLog] {
			//disable log
			entityManager->setActive(_backgroundLog, false); //background
			entityManager->setActive(_titleLog, false); //title
			entityManager->setActive(buttonCloseLog, false); //close button
			entityManager->setActive(buttonOpenLog, true); //open button
			entityManager->setActive(scrollingLog, false);
			entityManager->setActive(upScrollingLog, false);
			//text log
			//scroll buttons

			//activate open log button
			//_openLogButton->getMngr()->setActive(_openLogButton, true);
			//disable close log button
			//_closeLogButton->getMngr()->setActive(_closeLogButton, false);
			};

		ClickComponent* buttonOpenLogClick = entityManager->getComponent<ClickComponent>(buttonOpenLog);
		buttonOpenLogClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, scrollingLog, _titleLog, buttonCloseLog, buttonOpenLog]() {
			AudioManager::Instance().playSound(buttonSound);
			//open log
			roomEvent[LOGENABLE];

			//activate log
			entityManager->setActiveGroup(ecs::grp::LOG, true);
			entityManager->setActive(buttonOpenLog, false); //close button

			
			});
		entityManager->setActive(buttonOpenLog, true);

		ClickComponent* buttonCloseLogClick = entityManager->getComponent<ClickComponent>(buttonCloseLog);
		buttonCloseLogClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound, _backgroundLog, _titleLog, buttonCloseLog, buttonOpenLog]() {
			AudioManager::Instance().playSound(buttonSound);
			//close log
			roomEvent[LOGDESABLE];
			//disable log
			entityManager->setActiveGroup(ecs::grp::LOG, false);
			entityManager->setActive(buttonOpenLog, true); //open button
			});
		entityManager->setActive(buttonCloseLog, false);

		auto downScrollLogButton = entityManager->getComponent<ClickComponent>(scrollingLog);
		downScrollLogButton->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
			std::cout << "A" << std::endl;
			if (!ScrollComponentLog->isScrolling()) {
				ScrollComponentLog->Scroll(ScrollComponent::DOWN);
			}
			});

		auto upScrollLogButton = entityManager->getComponent<ClickComponent>(upScrollingLog);
		upScrollLogButton->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
			if (!ScrollComponentLog->isScrolling()) {
				ScrollComponentLog->Scroll(ScrollComponent::UP);
			}
			});


		//UI
		//Pause
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});

		//buttonOpenLogClick->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		//	while (ScrollComponentLog->numPhases() < (Game::Instance()->getDialogueManager()->getSceneLog()->getLogList()->size() / 5)) {
		//		ScrollComponentLog->addPhase();
		//	}
		//	});

		//_textbackgroundClick->connect(ClickComponent::JUST_CLICKED, [this, ScrollComponentLog]() {
		//	while (ScrollComponentLog->numPhases() < (Game::Instance()->getDialogueManager()->getSceneLog()->getLogList()->size() / 5)) {
		//		ScrollComponentLog->addPhase();
		//	}
		//	});

		dialogueManager->Init(0, entityFactory, entityManager, true, areaLayerManager, "SalaIntermedia1");
	}
	SDL_Delay(1000);

	roomEvent[FIRST_DIALOGUE]();
}

void MiddleRoomScene::resolvedPuzzle(int i)
{
}

void MiddleRoomScene::refresh()
{
	int aux = Game::Instance()->getDataManager()->GetActualScene();
	bool auxkei = Game::Instance()->getDataManager()->GetCharacterState(Character::KEISARA);
	bool auxlucy = Game::Instance()->getDataManager()->GetCharacterState(Character::LUCY);
	bool auxsol = Game::Instance()->getDataManager()->GetCharacterState(Character::SOL);
	switch (aux)
	{
	case SceneCount::MIDDLEROOM2:
		if (auxkei && auxlucy && auxsol) roomEvent[AFTER_ROOM1_GOOD3]();
		else if (!auxkei && auxlucy && auxsol)roomEvent[AFTER_ROOM1_BAD2]();
		else {
		#ifdef _DEBUG
			std::cout << "INVALID CHARACTERS STATE";
		#endif // DEBUG
		}
		break;
	case SceneCount::MIDDLEROOM3:
		if (auxkei && auxlucy && auxsol) roomEvent[AFTER_ROOM2_GOOD3]();
		else if (!auxkei && auxlucy && auxsol)roomEvent[AFTER_ROOM2_GOOD2]();
		else if (auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM2_BAD2]();
		else if (!auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM2_BAD1]();
		else {
		#ifdef _DEBUG
			std::cout << "INVALID CHARACTERS STATE";
		#endif // DEBUG
		}
		break;
	case SceneCount::END:
		if (auxkei && auxlucy && auxsol) roomEvent[AFTER_ROOM3_GOOD3]();
		else if (!auxkei && auxlucy && auxsol)roomEvent[AFTER_ROOM3_GOOD2SL]();
		else if (auxkei && auxlucy && !auxsol)roomEvent[AFTER_ROOM3_GOOD2KL]();
		else if (auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM3_BAD2SK]();
		else if (!auxkei && !auxlucy && auxsol)roomEvent[AFTER_ROOM3_BAD1S]();
		else if (auxkei && !auxlucy && !auxsol)roomEvent[AFTER_ROOM3_BAD1K]();
		else {
#ifdef _DEBUG
			std::cout << "INVALID CHARACTERS STATE";
#endif // DEBUG
		}
		break;
		break;
	default:
	#ifdef _DEBUG
		std::cout << "ERROR INVALID SCENECOUNT";
	#endif // DEBUG

		break;
	}
}

void MiddleRoomScene::unload()
{
}

void MiddleRoomScene::endDialogue()
{
	dialogueManager->setdisplayOnProcess(false);
	std::cout << "entro";
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	//Check the act room to load a specific room
	int aux = Game::Instance()->getDataManager()->GetActualScene();
	switch (aux)
	{
	case SceneCount::MIDDLEROOM1:
		Game::Instance()->getSceneManager()->loadScene(ROOM_1);
		break;
	case SceneCount::MIDDLEROOM2:
		Game::Instance()->getSceneManager()->loadScene(ROOM_2);
		break;
	case SceneCount::MIDDLEROOM3:
		Game::Instance()->getSceneManager()->loadScene(ROOM_3);
		break;
	case SceneCount::END:
		//Load the endScene WIP
		//Game::Instance()->getSceneManager()->loadScene(ROOM_1);
		break;
	default:
		break;
	}
	
}
