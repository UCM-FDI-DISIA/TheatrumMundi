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
#include "EventsInfo.h"
#include "Log.h"

MiddleRoomScene::MiddleRoomScene() :SceneRoomTemplate(), _eventToRead(SalaIntermedia1)
{
	roomEvent.resize(MIDDLEROOMEVENTSIZE);
	roomEvent[FIRST_DIALOGUE] = [this]() {
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

		//Log is always created on each middle room scene
		sceneLog = new Log();
		//Register log in dialogue manager
		Game::Instance()->getDialogueManager()->setSceneLog(sceneLog);

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
		auto scrollingLog = entityFactory->CreateInteractableEntityScroll(entityManager, "B6", EntityFactory::RECTAREA, Vector2D(1200, 100), Vector2D(0, 0), 50, 50, 270, areaLayerManager, sdlutils().height() / 50, 50, EntityFactory::SCROLLINVERSE, 1, EntityFactory::NODRAG, ecs::grp::LOG);
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
			entityManager->addComponent<WriteTextComponent<std::list<TextInfo>>>(_textLog, sdlutils().fonts().at("BASE"), colorText, sceneLog->getLogList()); //write text component
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

		

	}
	SDL_Delay(1000);

	roomEvent[FIRST_DIALOGUE]();
}

void MiddleRoomScene::resolvedPuzzle(int i)
{
}

void MiddleRoomScene::refresh()
{
}

void MiddleRoomScene::unload()
{
}

void MiddleRoomScene::endDialogue()
{
	entityManager->setActiveGroup(ecs::grp::DIALOGUE, false);
	//Check the act room to load a specific room
	//if(...)
	Game::Instance()->getSceneManager()->loadScene(ROOM_1);
}
