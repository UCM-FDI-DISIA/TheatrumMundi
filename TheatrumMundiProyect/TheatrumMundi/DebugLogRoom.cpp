#include "DebugLogRoom.h"
#include "../src/utils/Vector2D.h";
#include "../src/components/Transform.h"
#include "../src/components/Image.h"

#include "../src/components/WriteTextComponent.h"

#include "../src/components/LogComponent.h"

#include "../src/components/ClickComponent.h"
#include "../src/components/TriggerComponent.h"
#include "../src/components/DragComponent.h"
#include "../src/components/CircleArea2D.h"
#include "../src/components/RectArea2D.h"
#include "Area2DLayerManager.h"
#include "TextInfo.h"
#include "DialogueManager.h"

#include "../src/game/Game.h"

DebugLogRoom::DebugLogRoom() : SceneRoomTemplate(), _dialogueObj(nullptr)
{
	
}

DebugLogRoom::~DebugLogRoom()
{
}

void DebugLogRoom::init()
{
	if (!isStarted) {
		//Register scene in dialogue manager
		dialogueManager->setScene(this);

		//All Screen: Object to detect click on screen. Used to read displayed dialogue.
		auto _screenDetect = entityManager->addEntity();
		auto _screenDetectTr = entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);
		

		entityManager->addComponent<RectArea2D>(_screenDetect, areaLayerManager);
		ClickComponent* passDialog = entityManager->addComponent<ClickComponent>(_screenDetect);
		passDialog->connect(ClickComponent::JUST_CLICKED, [this, _screenDetect]()
			{
				if (!logActive && !isClickingButton) {
					//read dialogue only if it has to
					if (dialogueManager->getDisplayOnProcess())
					{
						dialogueManager->ReadDialogue("SalaIntermedia1");
						dialogueManager->ReadDialogue("SalaIntermedia1");
					}
					else
					{
						_screenDetect->getMngr()->setActive(_screenDetect, false);
					}
				}

			});

		//Create dialogue text entity. Object that renders dialogue Text on Screen
		auto _textTest = entityManager->addEntity();
		auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
		_dialogueObj = _textTest;


		//Create log
		auto _log = entityManager->addEntity();
		entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
		Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog"), 200); //background log
		
		LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent
		
		SDL_Color colorText = { 255, 255, 255, 255 };
		//WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog =
			//entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("BASE"), colorText, logComp->getLogList()); //write text component
		
		_log->getMngr()->setActive(_log, false); //hide log at the beggining

		//Register log in dialogue manager
		dialogueManager->setSceneLog(logComp);
			

		//Add writeText to dialogueManager
		SDL_Color colorDialog = { 255, 0, 0, 255 }; // Color = red
		WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog,dialogueManager->getShowText());

		//Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);
			
			
		//BUTTONS

		//Open log button
		auto _openLogButton = entityManager->addEntity();
		auto _openLogButtonTr = entityManager->addComponent<Transform>(_openLogButton, Vector2D(0, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_openLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_openLogButton, areaLayerManager);

		//Close log button
		auto _closeLogButton = entityManager->addEntity();
		auto _closeLogButtonTr = entityManager->addComponent<Transform>(_closeLogButton, Vector2D(1000, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_closeLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_closeLogButton, areaLayerManager);
		_closeLogButton->getMngr()->setActive(_closeLogButton, false);

		//Click component Open log button
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_openLogButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [_log, this, _openLogButton, _closeLogButton]()
			{
				//activate log
				_log->getMngr()->setActive(_log, true);
				logActive = true;

				//activate close log button
				_closeLogButton->getMngr()->setActive(_closeLogButton, true);

				//disable open log button
				_openLogButton->getMngr()->setActive(_openLogButton, false);
			});

		//Click component Close log button
		ClickComponent* clkClose = entityManager->addComponent<ClickComponent>(_closeLogButton);
		clkClose->connect(ClickComponent::JUST_CLICKED, [_log,this, _openLogButton, _closeLogButton]()
			{
				//disable log
				_log->getMngr()->setActive(_log, false);
				logActive = false;

				//activate open log button
				_openLogButton->getMngr()->setActive(_openLogButton, true);

				//disable close log button
				_closeLogButton->getMngr()->setActive(_closeLogButton, false);
			});
		

		//In this scene, in it's init, dialogue must start displaying on screen.
		dialogueManager->ReadDialogue("SalaIntermedia1");
		
		
	}
}

void DebugLogRoom::refresh()
{
	
}

void DebugLogRoom::unload()
{
	entityManager->~EntityManager();
}

void DebugLogRoom::showDialogue(bool show)
{
	if(show) _dialogueObj->getMngr()->setActive(_dialogueObj, true);
	else _dialogueObj->getMngr()->setActive(_dialogueObj, false);
}
