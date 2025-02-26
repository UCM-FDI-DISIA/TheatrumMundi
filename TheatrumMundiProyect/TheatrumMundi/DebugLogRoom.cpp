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
#include "TextInfo.h"

#include "../src/game/Game.h"

DebugLogRoom::DebugLogRoom() : SceneRoomTemplate()
{
	
}

DebugLogRoom::~DebugLogRoom()
{
}

void DebugLogRoom::init()
{
	if (!isStarted) {
			//All Screen
			auto _screenDetect = entityManager->addEntity();
			auto _screenDetectTr = entityManager->addComponent<Transform>(_screenDetect, Vector2D(0, 0), Vector2D(0, 0), sdlutils().width(), sdlutils().height(), 0);

			

			//Create dialogue text entity
			auto _textTest = entityManager->addEntity();
			auto _testTextTranform = entityManager->addComponent<Transform>(_textTest, Vector2D(600, 300), Vector2D(0, 0), 400, 200, 0);
			//_textTest->getMngr()->setActive(_textTest, false);

			//Create log
			auto _log = entityManager->addEntity();
			entityManager->addComponent<Transform>(_log, Vector2D(0, 0), Vector2D(0, 0), 1346, 748, 0); //transform
			LogComponent* logComp = entityManager->addComponent<LogComponent>(_log); //logComponent
			Image* imLog = entityManager->addComponent<Image>(_log, &sdlutils().images().at("fondoPruebaLog"), 200); //background log
			SDL_Color colorText = { 255, 255, 255, 255 };
			WriteTextComponent<std::list<std::pair<std::string, std::string>>>* writeLog =
				entityManager->addComponent<WriteTextComponent<std::list<std::pair<std::string, std::string>>>>(_log, sdlutils().fonts().at("BASE"), colorText, logComp->getLogList()); //write text component
			_log->getMngr()->setActive(_log, false); //hide log at the beggining

			//register log in dialogue manager
			Game::Instance()->getDialogueManager()->setSceneLog(logComp);
			
			

			entityManager->addComponent<RectArea2D>(_screenDetect);
			ClickComponent* passDialog = entityManager->addComponent<ClickComponent>(_screenDetect);
			passDialog->connect(ClickComponent::JUST_CLICKED, [this]()
				{
					if (!logActive&&!isClickingButton) {
						//read dialogue
						Game::Instance()->getDialogueManager()->ReadDialogue(SalaIntermediaEvento1);
						 }

				});

			//add writeText to dialogueManager
			SDL_Color colorDialog = { 255, 0, 0, 255 }; // Establecer el color (rojo)
			WriteTextComponent<TextInfo>* writeLogentityManager = entityManager->addComponent<WriteTextComponent<TextInfo>>(_textTest, sdlutils().fonts().at("BASE"), colorDialog, Game::Instance()->getDialogueManager()->getShowText());

			Game::Instance()->getDialogueManager()->setWriteTextComp(writeLogentityManager);
			
			


		//BUTTONS

		//Open log button
		auto _openLogButton = entityManager->addEntity();
		auto _openLogButtonTr = entityManager->addComponent<Transform>(_openLogButton, Vector2D(0, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_openLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_openLogButton);

		//Close log button
		auto _closeLogButton = entityManager->addEntity();
		auto _closeLogButtonTr = entityManager->addComponent<Transform>(_closeLogButton, Vector2D(1000, 0), Vector2D(0, 0), 200, 175, 0);
		entityManager->addComponent<Image>(_closeLogButton, &sdlutils().images().at("prueba"));

		entityManager->addComponent<RectArea2D>(_closeLogButton);
		_closeLogButton->getMngr()->setActive(_closeLogButton, false);

		//Click component clkOpen
		ClickComponent* clkOpen = entityManager->addComponent<ClickComponent>(_openLogButton);
		clkOpen->connect(ClickComponent::JUST_CLICKED, [_log, this, _openLogButton, _closeLogButton]()
			{
				std::cout << "CLICKED\n";
				_log->getMngr()->setActive(_log, true);
				logActive = true;
				_openLogButton->getMngr()->setActive(_openLogButton, false);
				_closeLogButton->getMngr()->setActive(_closeLogButton, true);
			});

		//Click component clkClose
		ClickComponent* clkClose = entityManager->addComponent<ClickComponent>(_closeLogButton);
		clkClose->connect(ClickComponent::JUST_CLICKED, [_log,this, _openLogButton, _closeLogButton]()
			{
				std::cout << "CLICKED\n";
				_log->getMngr()->setActive(_log, false);
				logActive = false;
				_openLogButton->getMngr()->setActive(_openLogButton, true);
				_closeLogButton->getMngr()->setActive(_closeLogButton, false);
			});
		


		//Start reading dialogue
		Game::Instance()->getDialogueManager()->ReadDialogue(SalaIntermediaEvento1);
		
		
	}
}

void DebugLogRoom::refresh()
{
}

void DebugLogRoom::unload()
{
	entityManager->~EntityManager();
}
