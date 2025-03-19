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
MiddleRoomScene::MiddleRoomScene() :SceneRoomTemplate(), _eventToRead(SalaIntermedia1)
{
	roomEvent.resize(MIDDLEROOMEVENTSIZE);
	roomEvent[FIRST_DIALOGUE] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	//ROOM1
	roomEvent[AFTER_ROOM1_GOOD3] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM1_BAD2] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM1_GOOD3] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	//ROOM2
	roomEvent[AFTER_ROOM2_GOOD3] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM2_GOOD2] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM2_BAD2] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM2_BAD1] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	//ROOM3
	roomEvent[AFTER_ROOM3_GOOD3] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM3_GOOD2SL] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM3_GOOD2KL] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM3_BAD2SK] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM3_BAD1K] = [this]() {
		startDialogue(SalaIntermedia1);
		};
	roomEvent[AFTER_ROOM3_BAD1S] = [this]() {
		startDialogue(SalaIntermedia1);
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
		Game::Instance()->getDialogueManager()->setScene(this);
		//MiddleRoomBkgrnd
		entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0, ecs::grp::MIDDLEROOM);


		//CharacterImage
		//auto characterimg = entityFactory->CreateImageEntity(entityManager, "Room", Vector2D(0, 0), Vector2D(0, 0), 500, 500, 0, ecs::grp::DIALOGUE);
		auto characterimg = entityManager->addEntity(grp::DIALOGUE);
		entityManager->addComponent<Transform>(characterimg, Vector2D(500, 50), Vector2D(0, 0), 1300 * 0.3, 2000 * 0.3, 0);
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
		entityManager->addComponent<Transform>(_textbackground, Vector2D(0, 0), Vector2D(0, 0), 1349, 748, 0);
		entityManager->addComponent<Image>(_textbackground, &sdlutils().images().at("Dialog"));
		entityManager->addComponent<RectArea2D>(_textbackground, areaLayerManager);

		entityManager->addComponent<ClickComponent>(_textbackground)->connect(ClickComponent::JUST_CLICKED, [this, _textbackground]()
			{
				if (!logActive) {
					//read dialogue only if it has to
					if (Game::Instance()->getDialogueManager()->getDisplayOnProcess())
					{
						Game::Instance()->getDialogueManager()->ReadDialogue(_eventToRead);
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

		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

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


		//UI
		//Pause
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});

		auto buttonLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(1200, 748 - (268 / 3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);

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
