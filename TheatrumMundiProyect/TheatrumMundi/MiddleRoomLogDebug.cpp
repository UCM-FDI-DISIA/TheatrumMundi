#include "MiddleRoomLogDebug.h"
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
#include "EventsInfo.h"
#include "Log.h"

MiddleRoomLogDebug::MiddleRoomLogDebug() :SceneRoomTemplate(), _eventToRead(SalaIntermedia1)
{
	roomEvent.resize(MIDDLEROOMEVENTSIZE);
	roomEvent[FIRST_DIALOGUE] = [this]() {
		startDialogue(SalaIntermedia1);
		};
}

MiddleRoomLogDebug::~MiddleRoomLogDebug()
{
}

void MiddleRoomLogDebug::init()
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

		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 0, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

		Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);

		
		
		//CREATE LOG

		//logic log
		Log* logScene = new Log();
		//Register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(logScene);

		/*
		auto _log = entityManager->addEntity(ecs::grp::LOG);
		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent
		//Register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(logComp);*/
		
		//background log
		auto _backgroundLog = entityManager->addEntity(ecs::grp::LOG);
		entityManager->addComponent<Transform>(_backgroundLog, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		entityManager->addComponent<Image>(_backgroundLog, &sdlutils().images().at("fondoPruebaLog"), 200); //background log
		entityManager->setActive(_backgroundLog, false);

		//title log
		auto _titleLog = entityManager->addEntity(ecs::grp::LOG);
		entityManager->addComponent<Transform>(_titleLog, Vector2D(50, 50), Vector2D(0, 0), 100, 100, 0);
		entityManager->addComponent<Image>(_titleLog, &sdlutils().images().at("B3"));
		entityManager->setActive(_titleLog, false);

		//scroll log buttons
		auto scrollingLog = entityFactory->CreateInteractableEntityScroll(entityManager, "decreaseButton", EntityFactory::RECTAREA, Vector2D(1000, 500), Vector2D(0, 0), 100, 100, 0, areaLayerManager, 20, 1000 /*- 1*/, EntityFactory::SCROLLNORMAL, 800/8, EntityFactory::NODRAG, ecs::grp::LOG);
		auto downScrollLog = entityManager->getComponent<ScrollComponent>(scrollingLog);

		auto upScrollingLog = entityFactory->CreateInteractableEntityScroll(entityManager, "incrementButton", EntityFactory::RECTAREA, Vector2D(1000, 0), Vector2D(0, 0), 100, 100, 0, areaLayerManager, 20, 1000 /*- 1*/, EntityFactory::SCROLLINVERSE, 800/8, EntityFactory::NODRAG, ecs::grp::LOG);
		auto upScrollLog = entityManager->getComponent<ScrollComponent>(upScrollingLog);

		//text log
		auto _textLog = entityManager->addEntity(ecs::grp::LOG);
		Transform* trTextLog = entityManager->addComponent<Transform>(_textLog, Vector2D(0, 0), Vector2D(0, 0), 800, 748, 0);
		Image* imTextLog = entityManager->addComponent<Image>(_textLog, &sdlutils().images().at("fondoPruebaLog"));
		SDL_Color colorText = { 255, 255, 255, 255 };
		WriteTextComponent<std::list<TextInfo>>* writeLog =
			entityManager->addComponent<WriteTextComponent<std::list<TextInfo>>>(_textLog, sdlutils().fonts().at("BASE"), colorText, logScene->getLogList()); //write text component
		entityManager->setActive(_textLog, false);

		downScrollLog->addElementToScroll(entityManager->getComponent<Transform>(_textLog));
		upScrollLog->addElementToScroll(entityManager->getComponent<Transform>(_textLog));

		//log buttons
		auto buttonOpenLog = entityFactory->CreateInteractableEntity(entityManager, "B7", EntityFactory::RECTAREA, Vector2D(1200, 748 - (268 / 3) - 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		auto buttonCloseLog = entityFactory->CreateInteractableEntity(entityManager, "B1", EntityFactory::RECTAREA, Vector2D(20, 500), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::LOG);

		


		roomEvent[LOGENABLE] = [this, _backgroundLog, _titleLog, buttonCloseLog, buttonOpenLog] {
			//activate log
			entityManager->setActive(_backgroundLog, true); //background
			entityManager->setActive(_titleLog, true); //title
			entityManager->setActive(buttonCloseLog, true); //close button
			entityManager->setActive(buttonOpenLog, false); //open button
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
		roomEvent[LOGDESABLE] = [this, _backgroundLog, _titleLog, buttonCloseLog, buttonOpenLog] {
			//disable log
			entityManager->setActive(_backgroundLog, false); //background
			entityManager->setActive(_titleLog, false); //title
			entityManager->setActive(buttonCloseLog, false); //close button
			entityManager->setActive(buttonOpenLog, true); //open button
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
			entityManager->setActive(buttonOpenLog, false); //open button
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
		downScrollLogButton->connect(ClickComponent::JUST_CLICKED, [this, downScrollLog]() {
			if (!downScrollLog->isScrolling()) {
				downScrollLog->Scroll(ScrollComponent::DOWN);
			}
			});
		entityManager->setActive(scrollingLog, false);

		auto upScrollLogButton = entityManager->getComponent<ClickComponent>(upScrollingLog);
		upScrollLogButton->connect(ClickComponent::JUST_CLICKED, [this, upScrollLog]() {
			if (!upScrollLog->isScrolling()) {
				upScrollLog->Scroll(ScrollComponent::UP);
			}
			});
		entityManager->setActive(upScrollingLog, false);

		//UI
		//Pause
		/*
		auto buttonPause = entityFactory->CreateInteractableEntity(entityManager, "B3", EntityFactory::RECTAREA, Vector2D(20, 20), Vector2D(0, 0), 90, 90, 0, areaLayerManager, EntityFactory::NODRAG, ecs::grp::UI);
		ClickComponent* buttonPauseClick = entityManager->getComponent<ClickComponent>(buttonPause);
		buttonPauseClick->connect(ClickComponent::JUST_CLICKED, [this, buttonSound]() {
			AudioManager::Instance().playSound(buttonSound);
			});*/

		
	}
	SDL_Delay(1000);

	roomEvent[FIRST_DIALOGUE]();
}

void MiddleRoomLogDebug::resolvedPuzzle(int i)
{
}

void MiddleRoomLogDebug::refresh()
{
}

void MiddleRoomLogDebug::unload()
{
}


void MiddleRoomLogDebug::endDialogue()
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	//Check the act room to load a specific room
	//if(...)
	//Game::Instance()->getSceneManager()->loadScene(ROOM_1);
}

